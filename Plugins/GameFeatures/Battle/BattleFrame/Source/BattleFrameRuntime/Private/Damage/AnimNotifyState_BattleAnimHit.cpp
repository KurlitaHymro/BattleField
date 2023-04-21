// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/AnimNotifyState_BattleAnimHit.h"
#include "BattleCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_BattleAnimHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	OwnerCharacter = Cast<ABattleCharacter>(MeshComp->GetOwner());
	WeaponSocketLocation = OwnerCharacter->Weapons[0]->GetSocketLocation(HitPoint); // TODO 用新的武器Actor

	HitActors.Empty();
}

void UAnimNotifyState_BattleAnimHit::NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	TArray<AActor*> HitTarget;

	TArray<AActor*> HitTargetIgnore;

	TArray<FHitResult> TickAllHitResult;

	FVector SocketLocation = OwnerCharacter->Weapons[0]->GetSocketLocation(HitPoint);
	UKismetSystemLibrary::BoxTraceMulti(OwnerCharacter->GetWorld(),
		WeaponSocketLocation, SocketLocation,
		FVector(1, 4, 20), // 密度，刃宽，刃长
		OwnerCharacter->Weapons[0]->GetComponentRotation(),
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
