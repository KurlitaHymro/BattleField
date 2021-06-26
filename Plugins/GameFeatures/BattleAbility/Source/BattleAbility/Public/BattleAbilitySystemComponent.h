// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BattleAbilitySystemComponent.generated.h"
 
/**
 * ƒ‹¡¶
 */
USTRUCT()
struct FBattleAbilityApplication
{
	GENERATED_BODY()

	// Ability set to grant
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameplayAbility> AbilityType;
};

/**
 *  Ù–‘
 */
USTRUCT()
struct FBattleAttributeApplication
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
 * 
 */
UCLASS()
class BATTLEABILITY_API UBattleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<FBattleAbilityApplication> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<FBattleAttributeApplication> DefaultAttributes;

protected:
	UPROPERTY(transient)
	TArray<struct FGameplayAbilitySpecHandle> AddedAbilityHandle;

	UPROPERTY(transient)
	TArray<class UAttributeSet*> AddedAttributeSet;

	void ClearAddedAbilities();
	void ClearAddedAttributes();
	void GrantDefaultAbilities();
	void GrantDefaultAttributes();
	void GrantDefaultAbilitiesAndAttributes();
};
