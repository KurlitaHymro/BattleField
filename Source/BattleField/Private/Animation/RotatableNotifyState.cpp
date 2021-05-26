// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/RotatableNotifyState.h"
#include "Character/BattlefieldCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

void URotatableNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyTick(MeshComp, Animation, TotalDuration);

	OwnerCharacter = Cast<ABattlefieldCharacterBase>(MeshComp->GetOwner());
	if (OwnerCharacter) {
		RotatorChangeRate = OwnerCharacter->InMotionRotatorChangeRate * MontageRotatorChangeRate;
	}
}

void URotatableNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyBegin(MeshComp, Animation, FrameDeltaTime);

	if (OwnerCharacter) {
		FRotator NewRotator = OwnerCharacter->GetExceptRotatorInMotion();
		FRotator CurRotator = OwnerCharacter->GetActorRotation();
		if (NewRotator != CurRotator) {
			FRotator DeltaRotator = UKismetMathLibrary::RInterpTo(CurRotator, NewRotator, FrameDeltaTime, RotatorChangeRate);
			OwnerCharacter->SetActorRotation(FRotator(CurRotator.Roll, DeltaRotator.Yaw, CurRotator.Pitch));
		}
	}

}

void URotatableNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

}