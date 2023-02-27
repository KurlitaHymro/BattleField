// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GrantAbilitySystemComponent.generated.h"


/**
 * 技能
 */
USTRUCT()
struct FAbilityApplication
{
	GENERATED_BODY()

	// Ability set to grant
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameplayAbility> AbilityType;
};

/**
 * 属性
 */
USTRUCT()
struct FAttributeApplication
{
	GENERATED_BODY()

	// Ability set to grant
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAttributeSet> AttributeSetType;

	// Data table reference to initialize the attributes with, if any (can be left unset)
	UPROPERTY(EditAnywhere)
	UDataTable* InitializationData = nullptr;
};

/**
 * As a GAS, Grant Default Abilities and Attributes when Pawn Init.
 * 取代GAS的ASC，在初始化时对Actor赋予默认的能力和属性。
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ABILITIESINPUTRUNTIME_API UGrantAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<FAbilityApplication> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<FAttributeApplication> DefaultAttributes;

protected:
	UPROPERTY(transient)
	TArray<struct FGameplayAbilitySpecHandle> GrantedAbilitiesHandle;

	UPROPERTY(transient)
	TArray<class UAttributeSet*> GrantedAttributesSet;

	void ClearGrantedAbilities();

	void ClearGrantedAttributes();

	void GrantDefaultAbilities();

	void GrantDefaultAttributes();

public:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	virtual void BeginDestroy() override;

	UFUNCTION()
	void OnPawnControllerChanged(APawn* Pawn, AController* NewController);
};
