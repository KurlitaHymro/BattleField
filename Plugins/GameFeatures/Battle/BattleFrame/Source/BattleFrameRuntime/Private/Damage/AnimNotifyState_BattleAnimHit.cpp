// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/AnimNotifyState_BattleAnimHit.h"
#include "BattleCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_BattleAnimHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	OwnerCharacter = Cast<ABattleCharacter>(MeshComp->GetOwner());

	HitPoint = { {FName("Blade0"), FName("Blade1")}, { } };

	WeaponSocketsLocations = { };
	for (int32 WeaponID = 0; WeaponID < OwnerCharacter->Weapons.Num(); WeaponID++)
	{
		TArray<FVector> WeaponSocket;
		for (int32 SocketID = 0; SocketID < HitPoint[WeaponID].Num(); SocketID++)
		{
			FVector SocketLocation = OwnerCharacter->Weapons[WeaponID]->GetSocketLocation(HitPoint[WeaponID][SocketID]);
			WeaponSocket.Emplace(SocketLocation);
		}
		WeaponSocketsLocations.Emplace(WeaponSocket);
	}
}

void UAnimNotifyState_BattleAnimHit::NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	TArray<AActor*> HitTarget;

	TArray<AActor*> HitTargetIgnore;

	TArray<FHitResult> hitResult;

	for (int32 WeaponID = 0; WeaponID < OwnerCharacter->Weapons.Num(); WeaponID++)
	{
		for (int32 SocketID = 0; SocketID < HitPoint[WeaponID].Num(); SocketID++)
		{
			FVector SocketLocation = OwnerCharacter->Weapons[WeaponID]->GetSocketLocation(HitPoint[WeaponID][SocketID]);
			UKismetSystemLibrary::BoxTraceMulti(OwnerCharacter->GetWorld(),
				WeaponSocketsLocations[WeaponID][SocketID], SocketLocation,
				FVector(1, 4, 20), // 1£¬ÈÐ¿í£¬ÈÐ³¤
				OwnerCharacter->Weapons[WeaponID]->GetComponentRotation(),
				ETraceTypeQuery::TraceTypeQuery4,
				false,
				HitTargetIgnore,
				EDrawDebugTrace::ForDuration,
				hitResult,
				true);
			WeaponSocketsLocations[WeaponID][SocketID] = SocketLocation;
		}
	}
}

void UAnimNotifyState_BattleAnimHit::NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}