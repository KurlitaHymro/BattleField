// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "Abilities/GameplayAbility.h"
#include "GameFeatureAction_AddMechanisms.generated.h"

USTRUCT(BlueprintType)
struct FAbilityInputMapping
{
	GENERATED_BODY()

	// Type of ability to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UGameplayAbility> AbilityType;

	// Input action to bind the ability to, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UInputAction> InputAction;
};

USTRUCT(BlueprintType)
struct FAttributeInitialMapping
{
	GENERATED_BODY()

	// Ability set to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UAttributeSet> AttributeSetType;

	// Data table referent to initialize the attributes with, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> InitializationData;
};

USTRUCT(meta = (DisplayName = "Add Actor Abilities Attributes Mechanisms"))
struct FGameFeatureAbilitiesAttributesEntry
{
	GENERATED_BODY()

	// The base actor class to add to
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSoftClassPtr<AActor> ActorClass;

	// List of abilities to grant to actors of the specified class
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<FAbilityInputMapping> GrantedAbilities;

	// List of attribute sets to grant to actors of the specified class 
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TArray<FAttributeInitialMapping> GrantedAttributes;
};

/**
 * 
 */
UCLASS(meta = (DisplayName = "Add Abilities&Attributes Mechanisms"))
class GAMEFEATURESEXPANSIONS_API UGameFeatureAction_AddMechanisms : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (TitleProperty = "ActorClass", ShowOnlyInnerProperties))
	TArray<FGameFeatureAbilitiesAttributesEntry> ActorAbilitiesAttributesList;

public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif
	//~ End UGameFeatureAction interface

	//~ Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//~ End UObject interface

private:
	void HandleGameInstanceStart(UGameInstance* GameInstance);

	void AddToWorld(const FWorldContext& WorldContext);

	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex);

	void ResetExtension();

	void AddAbilitiesAttributes(AActor* Actor, const FGameFeatureAbilitiesAttributesEntry& Entry);

	void RemoveAbilitiesAttributes(AActor* Actor);

private:
	template<class ComponentType>
	ComponentType* GetComponentForActor(AActor* Actor, const FGameFeatureAbilitiesAttributesEntry& Entry)
	{
		return Cast<ComponentType>(GetComponentForActor(ComponentType::StaticClass(), Actor, Entry));
	}
	UActorComponent* GetComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAbilitiesAttributesEntry& Entry);

private:
	FDelegateHandle GameInstanceStartHandle;

	TArray<TSharedPtr<struct FComponentRequestHandle>> ComponentRequests;
	
	struct FActorAbilitiesAttributes
	{
		TArray<FGameplayAbilitySpecHandle> Abilities;
		TArray<UAttributeSet*> Attributes;
	};
	TMap<AActor*, FActorAbilitiesAttributes> ActiveAbilitiesAttributes;
};
