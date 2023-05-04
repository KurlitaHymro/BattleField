// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TableRowDefine.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAnimDamageFactor : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class UAnimMontage> AnimMontageClass;

	UPROPERTY(EditDefaultsOnly)
	int32 HitTraceIndex;

	UPROPERTY(EditDefaultsOnly)
	float Factor;
};
