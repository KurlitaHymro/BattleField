// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API UAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	class AController* eventInstigator;

	class ABattlefieldCharacterBase* OwnerCharacter;

	class UMeshComponent* WeaponMesh;

	TArray<AActor*> HitTarget;

	TArray<AActor*> HitTargetIgnore;

	TArray<FVector> WeaponSlots;

	uint32 DectCnt;

	void UpdateHitResult(TArray<FHitResult>);

public:
	virtual void NotifyTick(USkeletalMeshComponent*, UAnimSequenceBase*, float) override;

	virtual void NotifyEnd(USkeletalMeshComponent*, UAnimSequenceBase*) override;

	virtual void NotifyBegin(USkeletalMeshComponent*, UAnimSequenceBase*, float) override;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	uint8 DectGap = 1;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	uint8 SlotNum = 2;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	uint8 DectX = 8;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	uint8 DectY = 4;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	uint8 DectZ = 2;
};
