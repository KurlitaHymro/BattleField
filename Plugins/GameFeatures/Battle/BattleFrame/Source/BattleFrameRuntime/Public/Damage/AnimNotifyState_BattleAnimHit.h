// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_BattleAnimHit.generated.h"

/**
 * 
 */

UCLASS()
class BATTLEFRAMERUNTIME_API UAnimNotifyState_BattleAnimHit : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName HitPoint;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;

private:
	class ABattleCharacter* OwnerCharacter;

	class AMeleeWeapon* Weapon;

	FVector WeaponSocketLocation;

	TArray<AActor*> HitActors;

private:
	void UpdateHitResult(TArray<FHitResult> AllHitResult);
};
