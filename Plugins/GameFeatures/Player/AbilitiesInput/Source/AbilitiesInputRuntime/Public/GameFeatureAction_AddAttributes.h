// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "Abilities/GameplayAbility.h"
#include "GameFeatureAction_AddAttributes.generated.h"

USTRUCT(BlueprintType)
struct FAttributeInfo
{
	GENERATED_BODY()
	// Ability set to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UAttributeSet> AttributeSetType;

	// Data table referent to initialize the attributes with, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> InitializationData;
};

USTRUCT(meta = (DisplayName = "Actor Attributes"))
struct FGameFeatureAttributesEntry
{
	GENERATED_BODY()

	// The base actor class to add to
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSoftClassPtr<AActor> ActorClass;

	// List of attribute sets to grant to actors of the specified class 
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TArray<FAttributeInfo> GrantedAttributes;
};

/**
 * 
 */
UCLASS(meta = (DisplayName = "Add Attributes"))
class ABILITIESINPUTRUNTIME_API UGameFeatureAction_AddAttributes : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Attributes", meta = (TitleProperty = "ActorClass", ShowOnlyInnerProperties))
	TArray<FGameFeatureAttributesEntry> ActorAttributes;

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
	ComponentType* GetComponentForActor(AActor* Actor, const FGameFeatureAttributesEntry& Entry)
	{
		return Cast<ComponentType>(GetComponentForActor(ComponentType::StaticClass(), Actor, Entry));
	}
	UActorComponent* GetComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAttributesEntry& Entry);

private:
	void HandleGameInstanceStart(UGameInstance* GameInstance);

	void AddToWorld(const FWorldContext& WorldContext);

	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex);

	void ResetExtension();

	void AddAttributes(AActor* Actor, const FGameFeatureAttributesEntry& Entry);

	void RemoveAttributes(AActor* Actor);

private:
	FDelegateHandle GameInstanceStartHandle;

	TArray<TSharedPtr<struct FComponentRequestHandle>> ComponentRequests;

	struct FActorAttributes
	{
		TArray<UAttributeSet*> Attributes;
	};
	TMap<AActor*, FActorAttributes> ActiveAttributes;
};
