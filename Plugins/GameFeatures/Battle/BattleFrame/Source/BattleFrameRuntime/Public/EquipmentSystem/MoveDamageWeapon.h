// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSystem/EquipBase.h"
#include "MoveDamageWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFRAMERUNTIME_API AMoveDamageWeapon : public AEquipBase
{
	GENERATED_BODY()

	friend class UAnimNotifyState_HitPointTrace;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "HitPoint Size"))
	TMap<FName, FVector> HitPoints;
};
