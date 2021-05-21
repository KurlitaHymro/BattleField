// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ComboNotifyState.h"
#include "Character/BattlefieldCharacterBase.h"

void UComboNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	OwnerCharacter = Cast<ABattlefieldCharacterBase>(MeshComp->GetOwner());
	if (OwnerCharacter) {
		OwnerCharacter->bRcvNextInput = true;
	}
}

void UComboNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UComboNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (OwnerCharacter) {
		OwnerCharacter->bRcvNextInput = false;
	}
}
