// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_DeriveCombo.generated.h"

USTRUCT(BlueprintType)
struct FAbilityDervieInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = DeriveCombo)
	TSubclassOf<UGameplayAbility> GameplayAbilityType;

	UPROPERTY(EditDefaultsOnly, Category = DeriveCombo)
	TArray<FGameplayTag> Conditions;
};

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
	TArray<FAbilityDervieInfo> DervieAbilities;

protected:
	UAbilitySystemComponent* AbilitySystemComponent;

	TArray<FActiveGameplayEffectHandle>	AppliedEffects;

	UFUNCTION(BlueprintCallable, Category = "AbilityDelegate")
	void OnCompleted();

	UFUNCTION(BlueprintCallable, Category = "AbilityDelegate")
	void OnBlendOut();

	UFUNCTION(BlueprintCallable, Category = "AbilityDelegate")
	void OnInterrupted();

	UFUNCTION(BlueprintCallable, Category = "AbilityDelegate")
	void OnCancelled();

	UFUNCTION()
	void OnTaskEnd();

	UFUNCTION()
	bool TryActivateDeriveAbility();
};
