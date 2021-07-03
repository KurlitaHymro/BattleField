// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputModComponent.h"
#include "GameplayAbilitySpec.h"
#include "EnhancedInputComponent.h"
#include "BattleAbilitiesInputComponent.generated.h"

USTRUCT()
struct FAbilityInputBinding
{
	GENERATED_BODY()

	int32  InputID = 0;
	uint32 OnPressedHandle = 0;
	uint32 OnReleasedHandle = 0;
	TArray<FGameplayAbilitySpecHandle> BoundAbilitiesStack;
};

/**
 * 将增强输入操作转化成InputID以激活/去激活技能
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLEABILITYATTRIBUTE_API UBattleAbilitiesInputComponent : public UPawnInputModComponent
{
	GENERATED_BODY()
	
};
