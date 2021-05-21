// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RotatableNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API URotatableNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	class ABattlefieldCharacterBase* OwnerCharacter;

	float RotatorChangeRate;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent*, UAnimSequenceBase*, float) override;

	virtual void NotifyTick(USkeletalMeshComponent*, UAnimSequenceBase*, float) override;

	virtual void NotifyEnd(USkeletalMeshComponent*, UAnimSequenceBase*) override;

public:
	UPROPERTY(Category = "State", EditAnywhere, BlueprintReadWrite)
	float MontageRotatorChangeRate = 1.0f;
};
