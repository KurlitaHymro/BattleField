// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddBattleMode.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Engine/AssetManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "AbilitySystemComponent.h"

#define LOCTEXT_NAMESPACE "BattleGameFeatures"
void UGameFeatureAction_AddBattleMode::OnGameFeatureActivating()
{
	if (!ensureAlways(ActiveExtensions.IsEmpty()) ||
		!ensureAlways(ComponentRequests.IsEmpty()))
	{
		Reset();
	}
	Super::OnGameFeatureActivating();
}

void UGameFeatureAction_AddBattleMode::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	Reset();
}

#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddBattleMode::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsValid())
	{
		auto AddBundleAsset = [&AssetBundleData](const FTopLevelAssetPath& Path)
		{
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, Path);
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, Path);
		};

		for (const auto& Entry : BattleModesList)
		{
			for (const auto& Ability : Entry.GrantedAbilities)
			{
				AddBundleAsset(Ability.AbilityType.ToSoftObjectPath().GetAssetPath());
				if (!Ability.InputAction.IsNull())
				{
					AddBundleAsset(Ability.InputAction.ToSoftObjectPath().GetAssetPath());
				}
			}

			for (const auto& Attributes : Entry.GrantedAttributes)
			{
				AddBundleAsset(Attributes.AttributeSetType.ToSoftObjectPath().GetAssetPath());
				if (!Attributes.InitializationData.IsNull())
				{
					AddBundleAsset(Attributes.InitializationData.ToSoftObjectPath().GetAssetPath());
				}
			}
		}
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddBattleMode::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const auto& Entry : BattleModesList)
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

void UGameFeatureAction_AddBattleMode::AddToWorld(const FWorldContext& WorldContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			int32 EntryIndex = 0;
			for (const FGameplayBattleModeEntry& Entry : BattleModesList)
			{
				if (!Entry.ActorClass.IsNull())
				{
					UGameFrameworkComponentManager::FExtensionHandlerDelegate AddDelegate = UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
						this, &UGameFeatureAction_AddBattleMode::HandleActorExtension, EntryIndex);
					TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = GFCM->AddExtensionHandler(Entry.ActorClass, AddDelegate);

					ComponentRequests.Add(ExtensionRequestHandle);
					EntryIndex++;
				}
			}
		}
	}
}

void UGameFeatureAction_AddBattleMode::Reset()
{
	while (!ActiveExtensions.IsEmpty())
	{
		RemoveActorBattleMode(ActiveExtensions.CreateIterator()->Key);
	}

	ComponentRequests.Empty();
}

void UGameFeatureAction_AddBattleMode::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex)
{
	if (BattleModesList.IsValidIndex(EntryIndex))
	{
		const auto& Entry = BattleModesList[EntryIndex];
		if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) // 防止重复添加，如果不好使就改成别的。4/9
		{
			AddActorBattleMode(Actor, Entry);
		}
		else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved)
		{
			RemoveActorBattleMode(Actor);
		}
	}
}

void UGameFeatureAction_AddBattleMode::AddActorBattleMode(AActor* Actor, const FGameplayBattleModeEntry& BattleModeEntry)
{
	if (UAbilitySystemComponent* ASC = FindOrAddComponentForActor<UAbilitySystemComponent>(Actor, BattleModeEntry))
	{
		FActorExtensions AddedExtensions;
		AddedExtensions.Abilities.Reserve(BattleModeEntry.GrantedAbilities.Num());
		AddedExtensions.Attributes.Reserve(BattleModeEntry.GrantedAttributes.Num());

		for (const auto& Ability : BattleModeEntry.GrantedAbilities)
		{
			if (!Ability.AbilityType.IsNull())
			{
				FGameplayAbilitySpec AbilitySpec(Ability.AbilityType.LoadSynchronous());
				FGameplayAbilitySpecHandle AbilityHandle = ASC->GiveAbility(AbilitySpec);

				//if (!Ability.InputAction.IsNull())
				//{
				//	UAbilityInputBindingComponent* InputComponent = FindOrAddComponentForActor<UAbilityInputBindingComponent>(Actor, BattleModeEntry);
				//	if (InputComponent)
				//	{
				//		InputComponent->SetInputBinding(Ability.InputAction.LoadSynchronous(), AbilityHandle);
				//	}
				//	else
				//	{
				//		UE_LOG(LogBattleFrame, Error, TEXT("Failed to find/add an ability input binding component to '%s' -- are you sure it's a pawn class?"), *Actor->GetPathName());
				//	}
				//}

				AddedExtensions.Abilities.Add(AbilityHandle);
			}
		}

		for (const auto& Attributes : BattleModeEntry.GrantedAttributes)
		{
			if (!Attributes.AttributeSetType.IsNull())
			{
				TSubclassOf<UAttributeSet> SetType = Attributes.AttributeSetType.LoadSynchronous();
				if (SetType)
				{
					UAttributeSet* AttributeSetInstance = NewObject<UAttributeSet>(ASC, SetType);
					if (!Attributes.InitializationData.IsNull())
					{
						UDataTable* InitData = Attributes.InitializationData.LoadSynchronous();
						if (InitData)
						{
							AttributeSetInstance->InitFromMetaDataTable(InitData);
						}
					}

					AddedExtensions.Attributes.Add(AttributeSetInstance);
					ASC->AddAttributeSetSubobject(AttributeSetInstance);
				}
			}
		}

		ActiveExtensions.Add(Actor, AddedExtensions);
	}
	else
	{
		UE_LOG(LogBattleFrame, Error, TEXT("Failed to find/add an ability component to '%s'. Abilities will not be granted."), *Actor->GetPathName());
	}
}

void UGameFeatureAction_AddBattleMode::RemoveActorBattleMode(AActor* Actor)
{
	if (FActorExtensions* ActorExtensions = ActiveExtensions.Find(Actor))
	{
		if (UAbilitySystemComponent* ASC = Actor->FindComponentByClass<UAbilitySystemComponent>())
		{
			for (UAttributeSet* AttributeSetInstance : ActorExtensions->Attributes)
			{
				ASC->RemoveSpawnedAttribute(AttributeSetInstance);
			}

			//UAbilityInputBindingComponent* InputComponent = Actor->FindComponentByClass<UAbilityInputBindingComponent>();
			for (FGameplayAbilitySpecHandle AbilityHandle : ActorExtensions->Abilities)
			{
				//if (InputComponent)
				//{
				//	InputComponent->ClearInputBinding(AbilityHandle);
				//}
				ASC->SetRemoveAbilityOnEnd(AbilityHandle);
			}
		}

		ActiveExtensions.Remove(Actor);
	}
}

UActorComponent* UGameFeatureAction_AddBattleMode::FindOrAddComponentForActor(UClass* ComponentType, AActor* Actor, const FGameplayBattleModeEntry& BattleModeEntry)
{
	UActorComponent* Component = Actor->FindComponentByClass(ComponentType);

	bool bMakeComponentRequest = (Component == nullptr);
	if (Component)
	{
		// Check to see if this component was created from a different `UGameFrameworkComponentManager` request.
		// `Native` is what `CreationMethod` defaults to for dynamically added components.
		if (Component->CreationMethod == EComponentCreationMethod::Native)
		{
			// Attempt to tell the difference between a true native component and one created by the GameFrameworkComponent system.
			// If it is from the UGameFrameworkComponentManager, then we need to make another request (requests are ref counted).
			UObject* ComponentArchetype = Component->GetArchetype();
			bMakeComponentRequest = ComponentArchetype->HasAnyFlags(RF_ClassDefaultObject);
		}
	}

	if (bMakeComponentRequest)
	{
		UWorld* World = Actor->GetWorld();
		UGameInstance* GameInstance = World->GetGameInstance();

		if (UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			TSharedPtr<FComponentRequestHandle> RequestHandle = GFCM->AddComponentRequest(BattleModeEntry.ActorClass, ComponentType);
			ComponentRequests.Add(RequestHandle);
		}

		if (!Component)
		{
			Component = Actor->FindComponentByClass(ComponentType);
			ensureAlways(Component);
		}
	}

	return Component;
}
#undef LOCTEXT_NAMESPACE