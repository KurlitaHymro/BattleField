// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddAttributes.h"
#include "AbilitiesInputComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Engine/AssetManager.h"
#include "Components/GameFrameworkComponentManager.h"

void UGameFeatureAction_AddAttributes::OnGameFeatureActivating()
{
	if (!ensureAlways(ComponentRequests.IsEmpty()) ||
		!ensureAlways(ActiveAttributes.IsEmpty()))
	{
		ResetExtension();
	}

	GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddAttributes::HandleGameInstanceStart);

	// Add to any worlds with associated game instances that have already been initialized
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		AddToWorld(WorldContext);
	}
}

void UGameFeatureAction_AddAttributes::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FWorldDelegates::OnStartGameInstance.Remove(GameInstanceStartHandle);

	ResetExtension();
}

#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddAttributes::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsValid())
	{
		auto AddBundleAsset = [&AssetBundleData](const FSoftObjectPath& SoftObjectPath)
		{
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, SoftObjectPath);
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, SoftObjectPath);
		};

		for (const auto& Entry : ActorAttributes)
		{
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
#define LOCTEXT_NAMESPACE "GameFeature_AddAttributes"
EDataValidationResult UGameFeatureAction_AddAttributes::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const auto& Entry : ActorAttributes)
	{
		if (Entry.ActorClass.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNullActor", "Null ActorClass at index {0} in AbilitiesList"), FText::AsNumber(EntryIndex)));
		}

		if (Entry.GrantedAttributes.IsEmpty())
		{
			Result = EDataValidationResult::Invalid;
			ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNoAddOns", "Empty GrantedAbilities and GrantedAttributes at index {0} in AbilitiesList"), FText::AsNumber(EntryIndex)));
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
#undef LOCTEXT_NAMESPACE
#endif

UActorComponent* UGameFeatureAction_AddAttributes::GetComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAttributesEntry& Entry)
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

void UGameFeatureAction_AddAttributes::HandleGameInstanceStart(UGameInstance* GameInstance)
{
	if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
	{
		AddToWorld(*WorldContext);
	}
}

void UGameFeatureAction_AddAttributes::AddToWorld(const FWorldContext& WorldContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance);
		if (GFCM)
		{
			int32 EntryIndex = 0;
			for (const auto& Entry : ActorAttributes)
			{
				UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAttributesDelegate =
					UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
						this,
						&UGameFeatureAction_AddAttributes::HandleActorExtension,
						EntryIndex);
				TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
					GFCM->AddExtensionHandler(
						Entry.ActorClass,
						AddAttributesDelegate);

				ComponentRequests.Add(ExtensionRequestHandle);
				EntryIndex++;
			}
		}
	}
}

void UGameFeatureAction_AddAttributes::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex)
{
	if (ActorAttributes.IsValidIndex(EntryIndex))
	{
		const auto& Entry = ActorAttributes[EntryIndex];
		if (EventName == UGameFrameworkComponentManager::NAME_ReceiverAdded ||
			EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
		{
			AddAttributes(Actor, Entry);
		}
		else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved ||
			EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
		{
			RemoveAttributes(Actor);
		}
	}
}

void UGameFeatureAction_AddAttributes::ResetExtension()
{
	while (!ActiveAttributes.IsEmpty())
	{
		auto ExtensionIt = ActiveAttributes.CreateIterator();
		RemoveAttributes(ExtensionIt->Key);
	}

	ComponentRequests.Empty();
}

void UGameFeatureAction_AddAttributes::AddAttributes(AActor* Actor, const FGameFeatureAttributesEntry& Entry)
{
	UAbilitySystemComponent* ASC = GetComponentForActor<UAbilitySystemComponent>(Actor, Entry);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Get UAbilitySystemComponent from '%s'"), *Actor->GetPathName());
		return;
	}

	TArray<UAttributeSet*> ActorExtension;

	ActorExtension.Reserve(Entry.GrantedAttributes.Num());
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

				ActorExtension.Add(SetObject);
			}
		}
	}

	ActiveAttributes.Add(Actor, ActorExtension);
}

void UGameFeatureAction_AddAttributes::RemoveAttributes(AActor* Actor)
{
	TArray<UAttributeSet*>* ActorExtension = ActiveAttributes.Find(Actor);
	if (ActorExtension)
	{
		UAbilitySystemComponent* ASC = Actor->FindComponentByClass<UAbilitySystemComponent>();
		if (!ASC)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Get UAbilitySystemComponent from '%s'"), *Actor->GetPathName());
			return;
		}

		for (auto Attribute : *ActorExtension)
		{
			ASC->GetSpawnedAttributes_Mutable().Remove(Attribute);
		}
	}

	ActiveAttributes.Remove(Actor);
}
