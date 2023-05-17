// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_AnimMove.h"
#include "BattleCharacter.h"
#include "AbilitySystemComponent.h"
#include "DamageSystem/AbilityTask_PlayMontageWaitEvent.h"

void UGameplayAbility_AnimMove::ApplySelfGameplayEffect()
{
	auto SelfMoveEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DefaultSelfMoveEffect, GetAbilityLevel());
	SelfEffectHandle = ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SelfMoveEffectSpecHandle);
}

void UGameplayAbility_AnimMove::RemoveSelfGameplayEffect()
{
	auto const ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	ASC->RemoveActiveGameplayEffect(SelfEffectHandle, -1);
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
	MontageTask = UAbilityTask_PlayMontageWaitEvent::CreatePlayMontageWaitEventProxy(
		this, NAME_None, MontageToPlay, EventTags, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale);

	MontageTask->OnCancelled.AddDynamic(this, &UGameplayAbility_AnimMove::OnCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_AnimMove::OnInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_AnimMove::OnBlendOut);
	MontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_AnimMove::OnCompleted);
	MontageTask->OnReceiveEvent.AddDynamic(this, &UGameplayAbility_AnimMove::OnReceiveEvent);
	MontageTask->Activate();
}

void UGameplayAbility_AnimMove::PlayDefaultAnimMoveMontage(FGameplayTagContainer EventTags, bool bStopWhenAbilityEnds)
{
	PlayMontageTask(EventTags, bStopWhenAbilityEnds, AnimMove.MontageToPlay, AnimMove.StartSection, AnimMove.Rate, AnimMove.AnimRootMotionTranslationScale);
}

void UGameplayAbility_AnimMove::OnCancelled_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(LogTemp, Error, TEXT("Cancelled"));
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_AnimMove::OnInterrupted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(LogTemp, Error, TEXT("Interrupted"));
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_AnimMove::OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(LogTemp, Error, TEXT("BlendOut"));
}

void UGameplayAbility_AnimMove::OnCompleted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(LogTemp, Error, TEXT("Completed"));
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_AnimMove::OnReceiveEvent_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Battle.Base.Event.HitDefault")))
	{
		auto ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
		if (ASC != nullptr)
		{
			auto DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DefaultDamageEffect, GetAbilityLevel());
			auto DataHandle = AssembleTargetData(EventData);
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DamageEffectSpecHandle, DataHandle);
		}
	}
	else if (EventTag == FGameplayTag::RequestGameplayTag(FName("Battle.Base.Event.HitShieldDefense")))
	{
		UE_LOG(LogTemp, Error, TEXT("Block"));
	}
}