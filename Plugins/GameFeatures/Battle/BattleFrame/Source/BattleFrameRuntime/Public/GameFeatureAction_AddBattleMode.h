// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayFrame/GameFeatureAction_AddToWorldBase.h"
#include "Abilities/GameplayAbility.h"
#include "GameFeatureAction_AddBattleMode.generated.h"

USTRUCT(BlueprintType)
struct FBattleAbilityMapping
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
struct FBattleAttributesMapping
{
	GENERATED_BODY()

	// Attribute set to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UAttributeSet> AttributeSetType;

	// Data table referent to initialize the attributes with, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> InitializationData;
};

USTRUCT()
struct FGameplayBattleModeEntry
{
	GENERATED_BODY()

	// The base actor class to add to
	UPROPERTY(EditAnywhere, Category = "Battle Character")
	TSoftClassPtr<AActor> ActorClass;

	// List of abilities to grant to actors of the specified class
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<FBattleAbilityMapping> GrantedAbilities;

	// List of attribute sets to grant to actors of the specified class 
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TArray<FBattleAttributesMapping> GrantedAttributes;
};

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Character Battle Mode"))
class UGameFeatureAction_AddBattleMode : public UGameFeatureAction_AddToWorldBase
{
	GENERATED_BODY()

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

	UPROPERTY(EditAnywhere, Category = "Battle Mode", meta = (TitleProperty = "ActorClass", ShowOnlyInnerProperties))
	TArray<FGameplayBattleModeEntry> BattleModesList;
protected:
	virtual void AddToWorld(const FWorldContext& WorldContext) override;
	void Reset();

	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex);
	void AddActorBattleMode(AActor* Actor, const FGameplayBattleModeEntry& BattleModeEntry);
	void RemoveActorBattleMode(AActor* Actor);

	template<class ComponentType>
	ComponentType* FindOrAddComponentForActor(AActor* Actor, const FGameplayBattleModeEntry& BattleModeEntry)
	{
		return Cast<ComponentType>(FindOrAddComponentForActor(ComponentType::StaticClass(), Actor, BattleModeEntry));
	}
	UActorComponent* FindOrAddComponentForActor(UClass* ComponentType, AActor* Actor, const FGameplayBattleModeEntry& BattleModeEntry);

private:
	struct FActorExtensions
	{
		TArray<int32> Abilities;
		TArray<UAttributeSet*> Attributes;
	};
	TMap<AActor*, FActorExtensions> ActiveExtensions;

	TArray<TSharedPtr<struct FComponentRequestHandle>> ComponentRequests;
};
