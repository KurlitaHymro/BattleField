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
	if (OwnerASC == nullptr || GameplayEffectClass == nullptr)
	{
		return;
	}

	auto GameplayEffectObject = GameplayEffectClass->ClassDefaultObject;
	auto FGameplayEffectContextHandle = OwnerASC->MakeEffectContext();
	if (GameplayEffectObject != nullptr && FGameplayEffectContextHandle.IsValid())
	{
		Handle = OwnerASC->ApplyGameplayEffectToSelf(Cast<UGameplayEffect>(GameplayEffectObject), 0, FGameplayEffectContextHandle);
	}
}

void UAnimNotifyState_ComboEnable::NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (OwnerASC == nullptr || !Handle.IsValid())
	{
		return;
	}
	OwnerASC->RemoveActiveGameplayEffect(Handle);
}