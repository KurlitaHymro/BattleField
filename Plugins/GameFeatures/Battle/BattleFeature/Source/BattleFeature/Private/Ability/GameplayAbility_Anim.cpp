// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayAbility_Anim.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGameplayAbility_Anim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateAbility Failed (Ability not be commited)"));
		K2_EndAbility();
		return;
	}

	AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

	if (MontageToPlay != nullptr && AnimInstance != nullptr && AnimInstance->GetActiveMontageInstance() == nullptr)
	{
		// Apply GameplayEffects
		TArray<const UGameplayEffect*> Effects;
		for (TSubclassOf<UGameplayEffect> EffectClass : GameplayEffectClassesWhileAnimating)
		{
			if (EffectClass)
			{
				Effects.Add(EffectClass->GetDefaultObject<UGameplayEffect>());
			}
		}

		if (Effects.Num() > 0)
		{
			for (const UGameplayEffect* Effect : Effects)
			{
				FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect, 1.f, MakeEffectContext(Handle, ActorInfo));
				if (EffectHandle.IsValid())
				{
					AppliedEffects.Add(EffectHandle);
				}
			}
		}

		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, MontageToPlay, PlayRate, SectionName, false, 1.0f, 0.f);
		MontageTask->OnCancelled.AddDynamic(this, &UGameplayAbility_Anim::OnCancelled);
		MontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_Anim::OnInterrupted);
		MontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_Anim::OnBlendOut);
		MontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_Anim::OnCompleted);
		MontageTask->Activate();
	}
	else
	{
		K2_EndAbility();
	}
}

void UGameplayAbility_Anim::OnCancelled()
{
	OnTaskEnd();
}

void UGameplayAbility_Anim::OnInterrupted()
{
	OnTaskEnd();
}

void UGameplayAbility_Anim::OnBlendOut()
{
	OnTaskEnd();
}

void UGameplayAbility_Anim::OnCompleted()
{
	OnTaskEnd();
}

void UGameplayAbility_Anim::OnTaskEnd()
{
	K2_EndAbility();

	if (AbilitySystemComponent)
	{
		for (FActiveGameplayEffectHandle Handle : AppliedEffects)
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
		}
	}
}

float UGameplayAbility_Anim::GetProgress()
{
	return bIsActive ? 1.f : 0.f;
}