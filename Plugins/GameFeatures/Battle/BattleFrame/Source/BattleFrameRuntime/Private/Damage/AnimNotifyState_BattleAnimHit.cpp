// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageSystem/AnimNotifyState_BattleAnimHit.h"
#include "BattleCharacter.h"
#include "BattleAbilitySystemComponent.h"
#include "EquipmentSystem/EquipmentSystemComponent.h"
#include "EquipmentSystem/MoveDamageWeapon.h"
#include "Kismet/KismetSystemLibrary.h"



void UAnimNotifyState_BattleAnimHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	OwnerCharacter = Cast<ABattleCharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	Weapon = Cast<AMoveDamageWeapon>(OwnerCharacter->GetEquipmentSystemComponent()->FindEquipFromSlot(EquipSlot));
	if (Weapon)
	{
		HitPointInfo = Weapon->HitPoints.Find(HitPoint);
		if (HitPointInfo)
		{
			HitPointHalfSize = *HitPointInfo;
			HitPointLocation = Weapon->GetMesh()->GetSocketLocation(HitPoint);
		}
	}

	HitActors.Empty();
}

void UAnimNotifyState_BattleAnimHit::NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (!OwnerCharacter || !HitPointInfo)
	{
		return;
	}

	// TODO: ����Tag
	TArray<AActor*> HitTargetIgnore;
	HitTargetIgnore.Add(OwnerCharacter);

	

	FVector CurrentLocation = Weapon->GetMesh()->GetSocketLocation(HitPoint);
	if (CurrentLocation.IsNearlyZero() || CurrentLocation.Equals(HitPointLocation))
	{
		return;
	}

	/* ������ */
	TArray<FHitResult> TickAllHitResult;
	UKismetSystemLibrary::BoxTraceMulti(OwnerCharacter->GetWorld(),
		HitPointLocation, // ���λ��
		CurrentLocation, // �ص��λ��
		HitPointHalfSize, // �ܶȣ��п��г�
		Weapon->GetMesh()->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery4,
		false,
		HitTargetIgnore,
		EDrawDebugTrace::ForDuration,
		TickAllHitResult,
		true);
	HitPointLocation = CurrentLocation;

	UpdateHitResult(TickAllHitResult);
}

void UAnimNotifyState_BattleAnimHit::NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation);

	HitPointInfo = nullptr;
}

void UAnimNotifyState_BattleAnimHit::UpdateHitResult(TArray<FHitResult> AllHitResult)
{
	UBattleAbilitySystemComponent* ASC = Cast<UBattleAbilitySystemComponent>(OwnerCharacter->GetAbilitySystemComponent());
	for (auto HitResult : AllHitResult)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActors.Contains(HitActor))
		{
			continue;
		}
		else
		{
			HitActors.Add(HitActor);
			if (ASC != nullptr)
			{
				ASC->HandleHitEvent(HitActor);
			}
		}
	}
}
