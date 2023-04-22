// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/AnimNotifyState_BattleAnimHit.h"
#include "BattleCharacter.h"
#include "MeleeWeapon.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_BattleAnimHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
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
		WeaponSocketLocation = Weapon->WeaponMeshComponent->GetSocketLocation(HitPoint);
	}

	HitActors.Empty();
}

void UAnimNotifyState_BattleAnimHit::NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (!OwnerCharacter || !Weapon)
	{
		return;
	}

	TArray<AActor*> HitTarget;

	TArray<AActor*> HitTargetIgnore;

	TArray<FHitResult> TickAllHitResult;

	FVector SocketLocation = Weapon->WeaponMeshComponent->GetSocketLocation(HitPoint);
	UKismetSystemLibrary::BoxTraceMulti(OwnerCharacter->GetWorld(),
		WeaponSocketLocation, SocketLocation,
		Weapon->HitPoints[0].TraceSize, // ÃÜ¶È£¬ÈÐ¿í£¬ÈÐ³¤
		Weapon->WeaponMeshComponent->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery4,
		false,
		HitTargetIgnore,
		EDrawDebugTrace::ForDuration,
		TickAllHitResult,
		true);
	WeaponSocketLocation = SocketLocation;

	UpdateHitResult(TickAllHitResult);
}

void UAnimNotifyState_BattleAnimHit::NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	HitActors.Empty();
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
