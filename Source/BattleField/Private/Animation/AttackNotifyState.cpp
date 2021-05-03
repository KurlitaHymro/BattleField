// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AttackNotifyState.h"
#include "Character/BattlefieldCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetSystemLibrary.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	DectCnt = 0;
	HitTargetIgnore = {};
	WeaponSlots = {};
	OwnerCharacter = Cast<ABattlefieldCharacterBase>(MeshComp->GetOwner());
	if (OwnerCharacter) {
		WeaponMesh = OwnerCharacter->GetWeaponMesh(0);
		if (!WeaponMesh) {
			UE_LOG(LogTemp, Error, TEXT("OwnerCharacter No WeaponMesh"));
			return;
		}
		HitTargetIgnore.Emplace(MeshComp->GetOwner());

		for (int slotIdx = 0; slotIdx < SlotNum; slotIdx++) {
			WeaponSlots.Emplace(WeaponMesh->GetSocketLocation(UCommonInterface::CombinFNameId("blade", slotIdx)));
		}
	} else {
		UE_LOG(RunLog, Error, TEXT("No OwnerCharacter"));
		return;
	}
}

void UAttackNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!OwnerCharacter || !WeaponMesh) {
		UE_LOG(RunLog, Error, TEXT("OwnerCharacter WeaponMesh Error"));
		return;
	}

	if ((++DectCnt) % DectGap) {
		return;
	}

	for (int slotIdx = 0; slotIdx < SlotNum; slotIdx++) {
		TArray<FHitResult> hitResult;
		/* 获取当前帧的slot位置 */
		FVector slotCurLocation = WeaponMesh->GetSocketLocation(UCommonInterface::CombinFNameId("blade", slotIdx));
		UKismetSystemLibrary::BoxTraceMulti(OwnerCharacter->GetWorld(),
			                                WeaponSlots[slotIdx],
			                                slotCurLocation,
			                                FVector(DectX,  // x - 武器刃方向 - 影响密度
												    DectY,  // y - 武器头方向 - 影响长度
												    DectZ), // z - 武器侧方向 - 影响宽度
			                                WeaponMesh->GetComponentRotation(),
			                                ETraceTypeQuery::TraceTypeQuery4,
			                                false,
			                                HitTargetIgnore,
			                                EDrawDebugTrace::ForDuration,
			                                hitResult,
			                                true);
		/* slot位置更新 */
		WeaponSlots[slotIdx] = slotCurLocation;

		UpdateHitResult(hitResult);
	}
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	HitTarget.Empty();
}

void UAttackNotifyState::UpdateHitResult(TArray<FHitResult> hitResult)
{
	for (int i = 0; i < hitResult.Num(); i++)
	{
		AActor* HitActor = hitResult[i].GetActor();
		if (!HitTarget.Contains(HitActor))
		{
			HitTarget.Add(HitActor);
			
			if (OwnerCharacter) {
				OwnerCharacter->HitActor(HitActor);
			}
		}
	}
}

