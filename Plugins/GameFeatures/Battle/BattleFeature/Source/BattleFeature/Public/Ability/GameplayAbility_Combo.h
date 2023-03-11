// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/GameplayAbility_Anim.h"
#include "GameplayAbility_Combo.generated.h"

USTRUCT(BlueprintType)
struct FActionChange
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = ComboAbility)
	class UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly, Category = ComboAbility)
	TSubclassOf<UGameplayAbility> GameplayAbility;
};

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UGameplayAbility_Combo : public UGameplayAbility_Anim
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = ComboAbility)
	TMap<class UInputAction*, TSubclassOf<UGameplayAbility>> DervieAbilities;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnCancelled() override;

	virtual void OnInterrupted() override;

	virtual void OnBlendOut() override;

	virtual void OnCompleted() override;

	void UpdateDervieAbilities();

};
