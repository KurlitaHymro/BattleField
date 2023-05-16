// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility_AnimMove.h"
#include "GameplayAbility_ComboAnimMoves.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UGameplayAbility_ComboAnimMoves : public UGameplayAbility_AnimMove
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CanComboTag = FGameplayTag::RequestGameplayTag(FName("Battle.Base.Status.AcceptComboInput"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAnimMoveMontage> ComboAnimMoves;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bComboEnable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ComboCount;

protected:
	virtual void PlayDefaultAnimMoveMontage(FGameplayTagContainer EventTags, bool bStopWhenAbilityEnds) override;

	virtual void OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData) override;

	UFUNCTION(BlueprintCallable, Category = Ability)
	void EnableComboWait();

protected:
	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnCanComboTagAdd();
	virtual void OnCanComboTagAdd_Implementation();

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnTryCombo(float WaitTime);
	virtual void OnTryCombo_Implementation(float WaitTime);

private:
	class UAbilityTask_WaitGameplayTagAdded* ComboStartTask;

	class UAbilityTask_WaitInputPress* WaitComboInputTask;
};
