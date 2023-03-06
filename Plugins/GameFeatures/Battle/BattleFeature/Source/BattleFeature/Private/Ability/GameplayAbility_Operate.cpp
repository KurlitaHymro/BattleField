// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayAbility_Operate.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

void UGameplayAbility_Operate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

	if (IdleAbilityType != nullptr && AnimInstance != nullptr && AnimInstance->GetActiveMontageInstance() == nullptr)
	{
		bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(IdleAbilityType);
		if (bSuccess)
		{
			K2_EndAbility();
		}
		//UAbilityTask_WaitInputRelease* task = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
		//task->OnRelease.AddDynamic(this, &UGameplayAbility_Operate::OnRelease);
		//task->Activate();
	}
	else
	{
		//UAbilityTask_WaitGameplayEffectRemoved::WaitForGameplayEffectRemoved();
	}
}

void UGameplayAbility_Operate::OnRelease(float ElapsedTime)
{
	K2_EndAbility();
	TryActivateIdleAbility();
}

bool UGameplayAbility_Operate::TryActivateIdleAbility()
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilityByClass(IdleAbilityType);
	}
	return false;
}