// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_HitTrace.h"
#include "BattleCharacter.h"
#include "BattleAbilitySystemComponent.h"
#include "EquipmentSystem/EquipmentSystemComponent.h"
#include "EquipmentSystem/MoveDamageWeapon.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_HitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// TODO:等数据表源搞好，下面这一坨想办法挪到加载时。
	if (0 == DamageFactor)
	{
		UDataTable* DataTable = LoadObject<UDataTable>(0, TEXT("DataTable'/SwordAndShield/DT_AnimMoveDamageConfig.DT_AnimMoveDamageConfig'"));
		TArray<FAnimMoveDamageConfig*> Rows;
		DataTable->GetAllRows<FAnimMoveDamageConfig>("", Rows);
		auto Iter = Rows.CreateIterator();
		while (Iter)
		{
			if ((*Iter)->HitMoveName == HitMoveName)
			{
				break;
			}
			++Iter;
		}
		if (Iter)
		{
			DamageFactor = (*Iter)->DamageFactor;
		}
		else
		{
			DamageFactor = 0;
		}
	}

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnerCharacter = nullptr;
	OwnerASC = nullptr;
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

	OwnerASC = OwnerCharacter->GetAbilitySystemComponent();
	if (OwnerASC == nullptr || DefaultSelfMoveEffect == nullptr || OwnerASC->GetOwner() == nullptr)
	{
		return;
	}
	FGameplayEffectContextHandle GameplayEffectContextHandle = OwnerASC->MakeEffectContext();
	FGameplayEffectSpecHandle SelfMoveEffectSpecHandle = OwnerASC->MakeOutgoingSpec(DefaultSelfMoveEffect, 0, GameplayEffectContextHandle);
	SelfMoveEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Battle.Data.MoveFactor")), DamageFactor);
	Handle = OwnerASC->ApplyGameplayEffectSpecToSelf(*SelfMoveEffectSpecHandle.Data.Get(), OwnerASC->GetPredictionKeyForNewAction());

	HitActors.Empty();
}

void UAnimNotifyState_HitTrace::NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!OwnerCharacter || !HitPointInfo)
	{
		return;
	}

	// TODO: 忽略Tag
	TArray<AActor*> HitTargetIgnore;
	HitTargetIgnore.Add(OwnerCharacter);



	FVector CurrentLocation = Weapon->GetMesh()->GetSocketLocation(HitPoint);
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

void UAnimNotifyState_HitTrace::NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	HitPointInfo = nullptr;
	if (OwnerASC == nullptr || !Handle.IsValid())
	{
		return;
	}
	OwnerASC->RemoveActiveGameplayEffect(Handle);
}

void UAnimNotifyState_HitTrace::UpdateHitResult(TArray<FHitResult> AllHitResult)
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