// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputModComponent.h"
#include "AbilitiesController.h"
#include "AbilitiesInputInterfaceComponent.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFRAMERUNTIME_API UAbilitiesInputInterfaceComponent : public UPawnInputModComponent, public IAbilitiesController
{
	GENERATED_BODY()
	
};
