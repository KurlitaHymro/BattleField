// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_RotatableEnable.h"
#include "BattleCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotifyState_RotatableEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnerCharacter = nullptr;
	OwnerCharacter = Cast<ABattleCharacter>(MeshComp->GetOwner());
}

void UAnimNotifyState_RotatableEnable::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (OwnerCharacter)
	{
		FRotator NewRotator = OwnerCharacter->GetExceptRotation();
		FRotator CurRotator = OwnerCharacter->GetActorRotation();
		if (NewRotator != CurRotator) {
			FRotator DeltaRotator = UKismetMathLibrary::RInterpTo(CurRotator, NewRotator, FrameDeltaTime, RotateSpeed);
			OwnerCharacter->SetActorRotation(DeltaRotator);
		}
	}
}

void UAnimNotifyState_RotatableEnable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

}
