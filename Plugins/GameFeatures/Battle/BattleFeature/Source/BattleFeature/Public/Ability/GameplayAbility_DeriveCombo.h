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

	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	FName SectionName;

	/** GameplayEffects to apply and then remove while the animation is playing */
	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClassesWhileAnimating;

	/** ≈……˙”≥…‰±Ì */
	UPROPERTY(EditDefaultsOnly, Category = DeriveCombo)
	TMap<TSubclassOf<class UGameplayAbility_Operate>, TSubclassOf<UGameplayAbility_DeriveCombo>> OperateDerive;

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent, TArray<struct FActiveGameplayEffectHandle> AppliedEffects);

	void GetGameplayEffectsWhileAnimating(TArray<const UGameplayEffect*>& OutEffects) const;
};
