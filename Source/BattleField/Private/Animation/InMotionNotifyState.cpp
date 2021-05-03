// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/InMotionNotifyState.h"
#include "Character/BattlefieldCharacterBase.h"

void UInMotionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	OwnerCharacter = Cast<ABattlefieldCharacterBase>(MeshComp->GetOwner());
	if (OwnerCharacter) {
		OwnerCharacter->bIsInMotion = true;
	}
}

void UInMotionNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UInMotionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (OwnerCharacter) {
		OwnerCharacter->bIsInMotion = false;
	}
}