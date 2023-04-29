// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_ComboEnable.h"
#include "BattleCharacter.h"
#include "AbilitySystemComponent.h"

void UAnimNotifyState_ComboEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	auto OwnerCharacter = Cast<ABattleCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		OwnerASC = OwnerCharacter->GetAbilitySystemComponent();
	}
	if (!OwnerASC || !GameplayEffectClass)
	{
		return;
	}
	Handle = OwnerASC->ApplyGameplayEffectToSelf(Cast<UGameplayEffect>(GameplayEffectClass->ClassDefaultObject), 0, OwnerASC->MakeEffectContext());
}

void UAnimNotifyState_ComboEnable::NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!OwnerASC)
	{
		return;
	}
	OwnerASC->RemoveActiveGameplayEffect(Handle);
}