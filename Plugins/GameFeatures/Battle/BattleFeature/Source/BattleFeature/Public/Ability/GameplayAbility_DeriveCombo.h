// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_DeriveCombo.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UGameplayAbility_DeriveCombo : public UGameplayAbility
{
	GENERATED_UCLASS_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = DeriveCombo)
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditDefaultsOnly, Category = DeriveCombo)
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, Category = DeriveCombo)
	FName SectionName;

	/** GameplayEffects to apply and then remove while the animation is playing */
	UPROPERTY(EditDefaultsOnly, Category = DeriveCombo)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClassesWhileAnimating;

	/** ≈……˙”≥…‰±Ì */
	UPROPERTY(EditDefaultsOnly, Category = DeriveCombo)
	TMap<TSubclassOf<class UGameplayAbility_Operate>, TSubclassOf<UGameplayAbility_DeriveCombo>> OperateDerive;

protected:
	UAbilitySystemComponent* AbilitySystemComponent;

	TArray<FActiveGameplayEffectHandle>	AppliedEffects;

	UFUNCTION()
	void OnTaskEnd();
};
