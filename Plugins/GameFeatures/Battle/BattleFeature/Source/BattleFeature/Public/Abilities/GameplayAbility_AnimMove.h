// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_AnimMove.generated.h"

USTRUCT(BlueprintType)
struct FAnimMoveMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName StartSection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Rate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AnimRootMotionTranslationScale = 1.0f;
};

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UGameplayAbility_AnimMove : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultSelfMoveEffect;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultDamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAnimMoveMontage AnimMove;

protected:
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual void ApplySelfGameplayEffect();

	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual FGameplayAbilityTargetDataHandle AssembleTargetData(const FGameplayEventData& EventData);

	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual void PlayMontageTask(
		FGameplayTagContainer EventTags,
		bool bStopWhenAbilityEnds,
		UAnimMontage* MontageToPlay,
		FName StartSection,
		float Rate = 1.0f,
		float AnimRootMotionTranslationScale = 1.0f);

	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual void PlayDefaultAnimMoveMontage(FGameplayTagContainer EventTags, bool bStopWhenAbilityEnds);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnCancelled_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnInterrupted(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnInterrupted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnBlendOut(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnCompleted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnReceiveEvent(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnReceiveEvent_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

protected:
	class UAbilityTask_PlayMontageWaitEvent* MontageTask;

	struct FActiveGameplayEffectHandle SelfEffectHandle;
};
