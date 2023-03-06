// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Operate.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UGameplayAbility_Operate : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TSubclassOf<UGameplayAbility> IdleAbilityType;

protected:
	UAbilitySystemComponent* AbilitySystemComponent;

	UFUNCTION()
	void OnRelease(float ElapsedTime);

	UFUNCTION()
	bool TryActivateIdleAbility();
};