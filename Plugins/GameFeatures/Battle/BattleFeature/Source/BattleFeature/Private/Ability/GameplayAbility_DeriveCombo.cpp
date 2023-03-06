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
		UE_LOG(LogTemp, Error, TEXT("ActivateAbility Failed (Ability not be commited)"));
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
	UE_LOG(LogTemp, Error, TEXT("OnCompleted"));
	OnTaskEnd();
}

void UGameplayAbility_DeriveCombo::OnBlendOut()
{
	UE_LOG(LogTemp, Error, TEXT("OnBlendOut"));
	if (TryActivateDeriveAbility())
	{
		OnTaskEnd();
	}
}

void UGameplayAbility_DeriveCombo::OnInterrupted()
{
	UE_LOG(LogTemp, Error, TEXT("OnInterrupted"));
	OnTaskEnd();
}

void UGameplayAbility_DeriveCombo::OnCancelled()
{
	UE_LOG(LogTemp, Error, TEXT("OnCancelled"));
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

bool UGameplayAbility_DeriveCombo::TryActivateDeriveAbility()
{
	if (AbilitySystemComponent)
	{
		// 所有派生技能按优先顺序填入默认值，因为可能不止一个派生技能满足要求，所以以满足条件的最高优先级技能为准。
		for (auto DervieInfo : DervieAbilities)
		{
			bool ConditionMet = false;
			for (auto Condition : DervieInfo.Conditions)
			{
				if (AbilitySystemComponent->HasMatchingGameplayTag(Condition))
				{
					ConditionMet = true;
				}
			}
			// 满足所有条件（至少一个）后，释放下个技能。
			if (ConditionMet)
			{
				return AbilitySystemComponent->TryActivateAbilityByClass(DervieInfo.GameplayAbilityType);
			}
		}
	}
	return false;
}