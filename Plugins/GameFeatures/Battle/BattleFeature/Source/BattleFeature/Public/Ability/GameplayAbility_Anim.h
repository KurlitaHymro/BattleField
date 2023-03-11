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
	GENERATED_UCLASS_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	TObjectPtr<UTexture> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	FName SectionName;

	/** GameplayEffects to apply and then remove while the animation is playing */
	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClassesWhileAnimating;

protected:
	ACharacter* AvatarCharacter;
	
	class UAbilitySystemComponent* AbilitySystemComponent;
	
	class UAbilitiesInputComponent* AbilitiesInputComponent;

	TArray<FActiveGameplayEffectHandle>	AppliedEffects;

	UFUNCTION(Category = "AbilityDelegate")
	virtual void OnCancelled();

	UFUNCTION(Category = "AbilityDelegate")
	virtual void OnInterrupted();

	UFUNCTION(Category = "AbilityDelegate")
	virtual void OnBlendOut();

	UFUNCTION(Category = "AbilityDelegate")
	virtual void OnCompleted();

	UFUNCTION()
	void OnTaskEnd();

	UFUNCTION(BlueprintCallable)
	float GetProgress();
};
