// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_MeleeMove.generated.h"

/**
 * 招式 MeleeMove 通过播放动画对武器的挥舞路径造成打击
 */
UCLASS()
class BATTLEFRAMERUNTIME_API UGameplayAbility_MeleeMove : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "AssembleTargetData"))
	FGameplayAbilityTargetDataHandle AssembleTargetData(const FGameplayEventData& EventData);
};
