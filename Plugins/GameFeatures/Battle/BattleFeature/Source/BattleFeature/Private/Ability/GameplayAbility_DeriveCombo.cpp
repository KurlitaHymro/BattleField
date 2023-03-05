// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayAbility_DeriveCombo.h"
#include "Animation/AnimInstance.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_DeriveCombo)

// --------------------------------------------------------------------------------------------------------------------------------------------------------
//
//	UGameplayAbility_DeriveCombo
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------

UGameplayAbility_DeriveCombo::UGameplayAbility_DeriveCombo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayRate = 1.f;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

void UGameplayAbility_DeriveCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
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

		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay, PlayRate, SectionName);
		MontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_DeriveCombo::OnCompleted);
		MontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_DeriveCombo::OnBlendOut);
		MontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_DeriveCombo::OnInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UGameplayAbility_DeriveCombo::OnCancelled);
		MontageTask->Activate();
	}
	else
	{
		K2_EndAbility();
	}
}

void UGameplayAbility_DeriveCombo::OnCompleted()
{
	OnTaskEnd();
	TryActivateDeriveAbility();
}

void UGameplayAbility_DeriveCombo::OnBlendOut()
{
}

void UGameplayAbility_DeriveCombo::OnInterrupted()
{
	OnTaskEnd();
}

void UGameplayAbility_DeriveCombo::OnCancelled()
{
}

void UGameplayAbility_DeriveCombo::OnTaskEnd()
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

void UGameplayAbility_DeriveCombo::TryActivateDeriveAbility()
{
	if (AbilitySystemComponent)
	{
		for (auto dervieInfo : DervieAbilities)
		{
			if (true)
			{
				bool result = AbilitySystemComponent->TryActivateAbilityByClass(dervieInfo.GameplayAbilityType);
				if (result)
				{
					return;
				}
			}
		}
	}
}