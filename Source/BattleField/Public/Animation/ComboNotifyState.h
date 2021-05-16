// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ComboNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API UComboNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
		
	class ABattlefieldCharacterBase* OwnerCharacter;

protected:
	virtual void NotifyTick(USkeletalMeshComponent*, UAnimSequenceBase*, float) override;

	virtual void NotifyBegin(USkeletalMeshComponent*, UAnimSequenceBase*, float) override;

	virtual void NotifyEnd(USkeletalMeshComponent*, UAnimSequenceBase*) override;
};
