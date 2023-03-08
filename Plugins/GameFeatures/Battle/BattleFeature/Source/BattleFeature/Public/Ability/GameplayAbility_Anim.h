// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Anim.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UGameplayAbility_Anim : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	//struct FTexture Icon;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	FName SectionName;

	/** GameplayEffects to apply and then remove while the animation is playing */
	UPROPERTY(EditDefaultsOnly, Category = DeriveCombo)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClassesWhileAnimating;

protected:
	UPROPERTY()
	float Progress;

	UAbilitySystemComponent* AbilitySystemComponent;

	TArray<FActiveGameplayEffectHandle>	AppliedEffects;

	UFUNCTION(BlueprintCallable, Category = "AbilityDelegate")
	void OnCancelled();

	UFUNCTION(BlueprintCallable, Category = "AbilityDelegate")
	void OnInterrupted();

	UFUNCTION(BlueprintCallable, Category = "AbilityDelegate")
	void OnBlendOut();

	UFUNCTION(BlueprintCallable, Category = "AbilityDelegate")
	void OnCompleted();

	UFUNCTION()
	void OnTaskEnd();

	UFUNCTION(BlueprintCallable)
	float GetProgress();
};
