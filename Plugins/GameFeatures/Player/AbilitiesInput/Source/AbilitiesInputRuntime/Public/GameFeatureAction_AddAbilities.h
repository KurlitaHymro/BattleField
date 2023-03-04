// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "Abilities/GameplayAbility.h"
#include "GameFeatureAction_AddAbilities.generated.h"

USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_BODY()

	// Type of ability to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UGameplayAbility> AbilityType;

	// Input action to bind the ability to, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UInputAction> InputAction;
};

USTRUCT(meta = (DisplayName = "Actor Abilities"))
struct FGameFeatureAbilitiesEntry
{
	GENERATED_BODY()

	// The base actor class to add to
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSoftClassPtr<AActor> ActorClass;

	// List of abilities to grant to actors of the specified class
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<FAbilityInfo> GrantedAbilities;
};

/**
 * 
 */
UCLASS(meta = (DisplayName = "Add Abilities"))
class ABILITIESINPUTRUNTIME_API UGameFeatureAction_AddAbilities : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (TitleProperty = "ActorClass", ShowOnlyInnerProperties))
	TArray<FGameFeatureAbilitiesEntry> ActorAbilities;

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
	template<class ComponentType>
	ComponentType* GetComponentForActor(AActor* Actor, const FGameFeatureAbilitiesEntry& Entry)
	{
		return Cast<ComponentType>(GetComponentForActor(ComponentType::StaticClass(), Actor, Entry));
	}
	UActorComponent* GetComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAbilitiesEntry& Entry);

private:
	void HandleGameInstanceStart(UGameInstance* GameInstance);

	void AddToWorld(const FWorldContext& WorldContext);

	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex);

	void ResetExtension();

	void AddAbilities(AActor* Actor, const FGameFeatureAbilitiesEntry& Entry);

	void RemoveAbilities(AActor* Actor);

private:
	FDelegateHandle GameInstanceStartHandle;

	TArray<TSharedPtr<struct FComponentRequestHandle>> ComponentRequests;

	TMap<AActor*, TArray<FGameplayAbilitySpecHandle>> ActiveAbilities;
};
