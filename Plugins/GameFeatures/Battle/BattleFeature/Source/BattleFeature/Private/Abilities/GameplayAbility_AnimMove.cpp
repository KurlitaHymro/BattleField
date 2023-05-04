// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_AnimMove.h"
#include "BattleCharacter.h"
#include "AbilitySystemComponent.h"
#include "DamageSystem/AbilityTask_PlayMontageWaitEvent.h"

void UGameplayAbility_AnimMove::ApplySelfGameplayEffect(float DamageFactor)
{
	auto SelfMoveEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DefaultSelfMoveEffect, GetAbilityLevel());
	SelfMoveEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Battle.Data.MoveFactor")), DamageFactor);
	SelfEffectHandle = K2_ApplyGameplayEffectSpecToOwner(SelfMoveEffectSpecHandle);
}

FGameplayAbilityTargetDataHandle UGameplayAbility_AnimMove::AssembleTargetData(const FGameplayEventData& EventData)
{
	FGameplayAbilityTargetDataHandle TargetData;
	AActor* Target = Cast<AActor>(EventData.Target);
	FGameplayAbilityTargetData_ActorArray* HitActorArrayData = new FGameplayAbilityTargetData_ActorArray();
	HitActorArrayData->TargetActorArray.Add(Target);
	TargetData.Add(HitActorArrayData);

	return TargetData;
}

void UGameplayAbility_AnimMove::PlayMontageTask(
	FGameplayTagContainer EventTags,
	bool bStopWhenAbilityEnds,
	UAnimMontage* MontageToPlay,
	FName StartSection,
	float Rate,
	float AnimRootMotionTranslationScale)
{
	auto Task = UAbilityTask_PlayMontageWaitEvent::CreatePlayMontageWaitEventProxy(
		this, NAME_None, MontageToPlay, EventTags, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale);

	Task->OnCancelled.AddDynamic(this, &UGameplayAbility_AnimMove::OnCancelled);
	Task->OnInterrupted.AddDynamic(this, &UGameplayAbility_AnimMove::OnInterrupted);
	Task->OnBlendOut.AddDynamic(this, &UGameplayAbility_AnimMove::OnBlendOut);
	Task->OnCompleted.AddDynamic(this, &UGameplayAbility_AnimMove::OnCompleted);
	Task->OnReceiveEvent.AddDynamic(this, &UGameplayAbility_AnimMove::OnReceiveEvent);
	Task->Activate();

	ApplySelfGameplayEffect(10.f);
}

void UGameplayAbility_AnimMove::PlayDefaultAnimMoveMontage(FGameplayTagContainer EventTags, bool bStopWhenAbilityEnds)
{
	PlayMontageTask(EventTags, bStopWhenAbilityEnds, AnimMove.MontageToPlay, AnimMove.StartSection, AnimMove.Rate, AnimMove.AnimRootMotionTranslationScale);
}

void UGameplayAbility_AnimMove::OnCancelled_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_EndAbility();
}

void UGameplayAbility_AnimMove::OnInterrupted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_EndAbility();
}

void UGameplayAbility_AnimMove::OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	auto ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	ASC->RemoveActiveGameplayEffect(SelfEffectHandle);
}

void UGameplayAbility_AnimMove::OnCompleted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_EndAbility();
}

void UGameplayAbility_AnimMove::OnReceiveEvent_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == FGameplayTag::EmptyTag) // TODO: 和ASC的Send配合修改以支持更多Tag
	{
		auto ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
		if (ASC != nullptr)
		{
			auto DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DefaultDamageEffect, GetAbilityLevel());
			auto DataHandle = AssembleTargetData(EventData);
			K2_ApplyGameplayEffectSpecToTarget(DamageEffectSpecHandle, DataHandle);
		}
	}
}