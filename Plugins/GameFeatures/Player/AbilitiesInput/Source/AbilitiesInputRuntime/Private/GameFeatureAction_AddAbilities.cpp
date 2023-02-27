// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddAbilities.h"
#include "AbilitiesInputComponent.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Engine/AssetManager.h"
#include "Components/GameFrameworkComponentManager.h"

void UGameFeatureAction_AddAbilities::OnGameFeatureActivating()
{
	if (!ensureAlways(ComponentRequests.IsEmpty()) ||
		!ensureAlways(ActiveAbilities.IsEmpty()))
	{
		ResetExtension();
	}

	GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddAbilities::HandleGameInstanceStart);

	// Add to any worlds with associated game instances that have already been initialized
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		AddToWorld(WorldContext);
	}
}

void UGameFeatureAction_AddAbilities::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FWorldDelegates::OnStartGameInstance.Remove(GameInstanceStartHandle);

	ResetExtension();
}

#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddAbilities::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsValid())
	{
		auto AddBundleAsset = [&AssetBundleData](const FSoftObjectPath& SoftObjectPath)
		{
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, SoftObjectPath);
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, SoftObjectPath);
		};

		for (const auto& Entry : ActorAbilities)
		{
			for (const auto& Ability : Entry.GrantedAbilities)
			{
				AddBundleAsset(Ability.AbilityType.ToSoftObjectPath());
				if (!Ability.InputAction.IsNull())
				{
					AddBundleAsset(Ability.InputAction.ToSoftObjectPath());
				}
			}
		}
	}
}
#endif

#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "GameFeature_AddAbilities"
EDataValidationResult UGameFeatureAction_AddAbilities::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const auto& Entry : ActorAbilities)
	{
		if (Entry.ActorClass.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNullActor", "Null ActorClass at index {0} in AbilitiesList"), FText::AsNumber(EntryIndex)));
		}

		if (Entry.GrantedAbilities.IsEmpty())
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

		++EntryIndex;
	}

	return Result;
}
#undef LOCTEXT_NAMESPACE
#endif

UActorComponent* UGameFeatureAction_AddAbilities::GetComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAbilitiesEntry& Entry)
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

void UGameFeatureAction_AddAbilities::HandleGameInstanceStart(UGameInstance* GameInstance)
{
	if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
	{
		AddToWorld(*WorldContext);
	}
}

void UGameFeatureAction_AddAbilities::AddToWorld(const FWorldContext& WorldContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance);
		if (GFCM)
		{
			int32 EntryIndex = 0;
			for (const auto& Entry : ActorAbilities)
			{
				UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate =
					UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
						this,
						&UGameFeatureAction_AddAbilities::HandleActorExtension,
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

void UGameFeatureAction_AddAbilities::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex)
{
	if (ActorAbilities.IsValidIndex(EntryIndex))
	{
		const auto& Entry = ActorAbilities[EntryIndex];
		if (EventName == UGameFrameworkComponentManager::NAME_ReceiverAdded ||
			EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
		{
			AddAbilities(Actor, Entry);
		}
		else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved ||
			EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
		{
			RemoveAbilities(Actor);
		}
	}
}

void UGameFeatureAction_AddAbilities::ResetExtension()
{
	while (!ActiveAbilities.IsEmpty())
	{
		auto ExtensionIt = ActiveAbilities.CreateIterator();
		RemoveAbilities(ExtensionIt->Key);
	}

	ComponentRequests.Empty();
}

void UGameFeatureAction_AddAbilities::AddAbilities(AActor* Actor, const FGameFeatureAbilitiesEntry& Entry)
{
	UAbilitySystemComponent* ASC = GetComponentForActor<UAbilitySystemComponent>(Actor, Entry);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Get UAbilitySystemComponent from '%s'"), *Actor->GetPathName());
		return;
	}

	FActorAbilities ActorExtension;
	
	ActorExtension.Abilities.Reserve(Entry.GrantedAbilities.Num());
	for (const auto& Ability : Entry.GrantedAbilities)
	{
		if (!Ability.AbilityType.IsNull())
		{
			FGameplayAbilitySpec AbilitySpec(Ability.AbilityType.LoadSynchronous());
			FGameplayAbilitySpecHandle AbilityHandle = ASC->GiveAbility(AbilitySpec);
	
			if (!Ability.InputAction.IsNull())
			{
				UAbilitiesInputComponent* AIC = GetComponentForActor<UAbilitiesInputComponent>(Actor, Entry);
				if (AIC)
				{
					AIC->SetupBinding(Ability.InputAction.LoadSynchronous(), AbilityHandle);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to Get UBattleAbilitiesInputComponent from '%s'"), *Actor->GetPathName());
					continue;
				}
			}
	
			ActorExtension.Abilities.Add(AbilityHandle);
		}
	}
	
	ActiveAbilities.Add(Actor, ActorExtension);
}

void UGameFeatureAction_AddAbilities::RemoveAbilities(AActor* Actor)
{
	FActorAbilities* ActorExtension = ActiveAbilities.Find(Actor);
	if (ActorExtension)
	{
		UAbilitySystemComponent* ASC = Actor->FindComponentByClass<UAbilitySystemComponent>();
		if (!ASC)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Get UAbilitySystemComponent from '%s'"), *Actor->GetPathName());
			return;
		}
	
		UAbilitiesInputComponent* AIC = Actor->FindComponentByClass<UAbilitiesInputComponent>();
		for (auto Ability : ActorExtension->Abilities)
		{
			if (AIC)
			{
				AIC->TeardownAbilityBinding(Ability);
			}
			ASC->SetRemoveAbilityOnEnd(Ability);
		}
	}
	
	ActiveAbilities.Remove(Actor);
}