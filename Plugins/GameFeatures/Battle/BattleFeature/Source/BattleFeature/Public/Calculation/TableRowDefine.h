// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TableRowDefine.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAnimMoveConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName HitMoveName;

	UPROPERTY(EditDefaultsOnly)
	float DamageFactor;

	UPROPERTY(EditDefaultsOnly)
	float ShockLevel;

	UPROPERTY(EditDefaultsOnly)
	float StoicLevel;
};
