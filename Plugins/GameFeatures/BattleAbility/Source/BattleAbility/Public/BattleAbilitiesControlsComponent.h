// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/Components/PawnControlsComponent.h"
#include "BattleAbilitiesControlsComponent.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEABILITY_API UBattleAbilitiesControlsComponent : public UPawnControlsComponent
{
	GENERATED_BODY()

protected:
	virtual void SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent) override;
};
