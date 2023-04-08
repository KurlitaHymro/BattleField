// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputModComponent.h"
#include "AbilityInputBindingComponent.generated.h"

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
 * 
 */
UCLASS()
class BATTLEFRAMERUNTIME_API UAbilityInputBindingComponent : public UPawnInputModComponent
{
	GENERATED_BODY()
	

};
