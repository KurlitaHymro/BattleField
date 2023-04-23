// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/AnimNotifyState_BattleAnimHit.h"
#include "BattleCharacter.h"
#include "MeleeWeapon.h"
#include "Kismet/KismetSystemLibrary.h"



void UAnimNotifyState_BattleAnimHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	OwnerCharacter = Cast<ABattleCharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	Weapon = Cast<AMeleeWeapon>(OwnerCharacter->Weapon);
	if (Weapon)
	{
		HitPointInfo = Weapon->HitPoints.Find(HitPoint);
		if (HitPointInfo)
		{
			HitPointHalfSize = *HitPointInfo;
			HitPointLocation = Weapon->WeaponMeshComponent->GetSocketLocation(HitPoint);
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

	// TODO: 忽略Tag
	TArray<AActor*> HitTargetIgnore;
	HitTargetIgnore.Add(OwnerCharacter);

	

	FVector CurrentLocation = Weapon->WeaponMeshComponent->GetSocketLocation(HitPoint);
	if (CurrentLocation.IsNearlyZero() || CurrentLocation.Equals(HitPointLocation))
	{
		return;
	}

	/* 刀光检测 */
	TArray<FHitResult> TickAllHitResult;
	UKismetSystemLibrary::BoxTraceMulti(OwnerCharacter->GetWorld(),
		HitPointLocation, // 起点位置
		CurrentLocation, // 重点点位置
		HitPointHalfSize, // 密度，刃宽，刃长
		Weapon->WeaponMeshComponent->GetComponentRotation(),
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
			ABattleCharacter* HitCharacter = Cast<ABattleCharacter>(HitActor);
			if (HitCharacter)
			{
				OwnerCharacter->CauseDamage(10.f, FDamageEvent(), HitCharacter->Controller, nullptr);
			}
		}
	}
}
