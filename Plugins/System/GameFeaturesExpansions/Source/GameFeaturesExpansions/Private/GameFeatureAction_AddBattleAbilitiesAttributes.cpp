// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddBattleAbilitiesAttributes.h"
#include "Engine/AssetManager.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "GameFeaturesExpansions.h"
#include "BattleAbilitiesInputComponent.h"
#include "BattleSystemComponent.h"

#define LOCTEXT_NAMESPACE "GameFeaturesExpansions"

void UGameFeatureAction_AddBattleAbilitiesAttributes::OnGameFeatureActivating()
{
	if (!ensureAlways(ComponentRequests.IsEmpty()) ||
		!ensureAlways(ActiveAbilitiesAttributes.IsEmpty()))
	{
		ResetExtension();
	}

	GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddBattleAbilitiesAttributes::HandleGameInstanceStart);

	// Add to any worlds with associated game instances that have already been initialized
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		AddToWorld(WorldContext);
	}
}

void UGameFeatureAction_AddBattleAbilitiesAttributes::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FWorldDelegates::OnStartGameInstance.Remove(GameInstanceStartHandle);

	ResetExtension();
}

#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddBattleAbilitiesAttributes::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsValid())
	{
		auto AddBundleAsset = [&AssetBundleData](const FSoftObjectPath& SoftObjectPath)
		{
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, SoftObjectPath);
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, SoftObjectPath);
		};

		for (const auto& Entry : ActorAbilitiesAttributesList)
		{
			for (const auto& Ability : Entry.GrantedAbilities)
			{
				AddBundleAsset(Ability.AbilityType.ToSoftObjectPath());
				if (!Ability.InputAction.IsNull())
				{
					AddBundleAsset(Ability.InputAction.ToSoftObjectPath());
				}
			}

			for (const auto& Attributes : Entry.GrantedAttributes)
			{
				AddBundleAsset(Attributes.AttributeSetType.ToSoftObjectPath());
				if (!Attributes.InitializationData.IsNull())
				{
					AddBundleAsset(Attributes.InitializationData.ToSoftObjectPath());
				}
			}
		}
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddBattleAbilitiesAttributes::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const auto& Entry : ActorAbilitiesAttributesList)
	{
		if (Entry.ActorClass.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNullActor", "Null ActorClass at index {0} in AbilitiesList"), FText::AsNumber(EntryIndex)));
		}

		if (Entry.GrantedAbilities.IsEmpty() && Entry.GrantedAttributes.IsEmpty())
		{
			Result = EDataValidationResult::Invalid;
			ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNoAddOns", "Empty GrantedAbilities and GrantedAttributes at index {0} in AbilitiesList"), FText::AsNumber(EntryIndex)));
		}

		int32 AbilityIndex = 0;
		for (const auto& Ability : Entry.GrantedAbilities)
		{
			if (Ability.AbilityType.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNullAbility", "Null AbilityType at index {0} in AbilitiesList[{1}].GrantedAbilities"), FText::AsNumber(AbilityIndex), FText::AsNumber(EntryIndex)));
			}
			++AbilityIndex;
		}

		int32 AttributesIndex = 0;
		for (const auto& Attributes : Entry.GrantedAttributes)
		{
			if (Attributes.AttributeSetType.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNullAttributeSet", "Null AttributeSetType at index {0} in AbilitiesList[{1}].GrantedAttributes"), FText::AsNumber(AttributesIndex), FText::AsNumber(EntryIndex)));
			}
			++AttributesIndex;
		}

		++EntryIndex;
	}

	return Result;
}
#endif

void UGameFeatureAction_AddBattleAbilitiesAttributes::HandleGameInstanceStart(UGameInstance* GameInstance)
{
	if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
	{
		AddToWorld(*WorldContext);
	}
}

void UGameFeatureAction_AddBattleAbilitiesAttributes::AddToWorld(const FWorldContext& WorldContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance);
		if (GFCM)
		{
			int32 EntryIndex = 0;
			for (const auto& Entry : ActorAbilitiesAttributesList)
			{
				UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate =
					UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
						this,
						&UGameFeatureAction_AddBattleAbilitiesAttributes::HandleActorExtension,
						EntryIndex);
				TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
					GFCM->AddExtensionHandler(
						Entry.ActorClass,
						AddAbilitiesDelegate);

				ComponentRequests.Add(ExtensionRequestHandle);
				EntryIndex++;
			}
		}
	}
}

void UGameFeatureAction_AddBattleAbilitiesAttributes::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex)
{
	if (ActorAbilitiesAttributesList.IsValidIndex(EntryIndex))
	{
		const auto& Entry = ActorAbilitiesAttributesList[EntryIndex];
		if (EventName == UGameFrameworkComponentManager::NAME_ReceiverAdded ||
			EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
		{
			AddAbilitiesAttributes(Actor, Entry);
		}
		else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved ||
			EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
		{
			RemoveAbilitiesAttributes(Actor);
		}
	}
}

void UGameFeatureAction_AddBattleAbilitiesAttributes::ResetExtension()
{
	while (!ActiveAbilitiesAttributes.IsEmpty())
	{
		auto ExtensionIt = ActiveAbilitiesAttributes.CreateIterator();
		RemoveAbilitiesAttributes(ExtensionIt->Key);
	}

	ComponentRequests.Empty();
}

void UGameFeatureAction_AddBattleAbilitiesAttributes::AddAbilitiesAttributes(AActor* Actor, const FGameFeatureAbilitiesAttributesEntry& Entry)
{
	UAbilitySystemComponent* ASC = GetComponentForActor<UAbilitySystemComponent>(Actor, Entry);
	if (!ASC)
	{
		UE_LOG(LogExpansion, Error, TEXT("Failed to Get UAbilitySystemComponent from '%s'"), *Actor->GetPathName());
		return;
	}

	FActorAbilitiesAttributes ActorExtension;

	ActorExtension.Abilities.Reserve(Entry.GrantedAbilities.Num());
	for (const auto& Ability : Entry.GrantedAbilities)
	{
		if (!Ability.AbilityType.IsNull())
		{
			FGameplayAbilitySpec AbilitySpec(Ability.AbilityType.LoadSynchronous());
			FGameplayAbilitySpecHandle AbilityHandle = ASC->GiveAbility(AbilitySpec);

			if (!Ability.InputAction.IsNull())
			{
				UBattleAbilitiesInputComponent* BAIC = GetComponentForActor<UBattleAbilitiesInputComponent>(Actor, Entry);
				if (BAIC)
				{
					BAIC->SetupBinding(Ability.InputAction.LoadSynchronous(), AbilityHandle);
				}
				else
				{
					UE_LOG(LogExpansion, Error, TEXT("Failed to Get UBattleAbilitiesInputComponent from '%s'"), *Actor->GetPathName());
					continue;
				}
			}

			ActorExtension.Abilities.Add(AbilityHandle);
		}
	}

	ActorExtension.Attributes.Reserve(Entry.GrantedAttributes.Num());
	for (const auto& Attribute : Entry.GrantedAttributes)
	{
		if (!Attribute.AttributeSetType.IsNull())
		{
			TSubclassOf<UAttributeSet> SetType = Attribute.AttributeSetType.LoadSynchronous();
			if (SetType)
			{
				UAttributeSet* SetObject = NewObject<UAttributeSet>(ASC, SetType);

				if (!Attribute.InitializationData.IsNull())
				{
					UDataTable* InitialData = Attribute.InitializationData.LoadSynchronous();
					if (InitialData)
					{
						SetObject->InitFromMetaDataTable(InitialData);
					}
				}
				ASC->AddAttributeSetSubobject(SetObject);

				ActorExtension.Attributes.Add(SetObject);
			}
		}
	}

	ActiveAbilitiesAttributes.Add(Actor, ActorExtension);
}

void UGameFeatureAction_AddBattleAbilitiesAttributes::RemoveAbilitiesAttributes(AActor* Actor)
{
	FActorAbilitiesAttributes* ActorExtension = ActiveAbilitiesAttributes.Find(Actor);
	if (ActorExtension)
	{
		UAbilitySystemComponent* ASC = Actor->FindComponentByClass<UAbilitySystemComponent>();
		if (!ASC)
		{
			UE_LOG(LogExpansion, Error, TEXT("Failed to Get UAbilitySystemComponent from '%s'"), *Actor->GetPathName());
			return;
		}

		UBattleAbilitiesInputComponent* BAIC = Actor->FindComponentByClass<UBattleAbilitiesInputComponent>();
		for (auto Ability : ActorExtension->Abilities)
		{
			if (BAIC)
			{
				BAIC->TeardownAbilityBinding(Ability);
			}
			ASC->SetRemoveAbilityOnEnd(Ability);
		}

		for (auto Attribute : ActorExtension->Attributes)
		{
			ASC->GetSpawnedAttributes_Mutable().Remove(Attribute);
		}
	}

	ActiveAbilitiesAttributes.Remove(Actor);
}

UActorComponent* UGameFeatureAction_AddBattleAbilitiesAttributes::GetComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAbilitiesAttributesEntry& Entry)
{
	UActorComponent* Component = Actor->FindComponentByClass(ComponentType);

	bool Request = (Component == nullptr);

	if (Component)
	{
		if (Component->CreationMethod == EComponentCreationMethod::Native)
		{
			UObject* ComponentArchetype = Component->GetArchetype();
			Request = ComponentArchetype->HasAnyFlags(RF_ClassDefaultObject);
		}
	}

	if (Request)
	{
		UGameInstance* GameInstance = Actor->GetWorld()->GetGameInstance();
		UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance);
		if (GFCM)
		{
			TSharedPtr<FComponentRequestHandle> RequestHandle = GFCM->AddComponentRequest(Entry.ActorClass, ComponentType);
			ComponentRequests.Add(RequestHandle);
		}

		if (Component == nullptr)
		{
			Component = Actor->FindComponentByClass(ComponentType);
			ensureAlways(Component);
		}
	}

	return Component;
}

#undef LOCTEXT_NAMESPACE