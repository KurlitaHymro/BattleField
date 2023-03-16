// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayAbility_Anim.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitiesInputComponent.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_Anim)

UGameplayAbility_Anim::UGameplayAbility_Anim(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayRate = 1.f;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

void UGameplayAbility_Anim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, OwnerInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateAbility Failed (Ability not be commited)"));
		return;
	}

	AbilitySpecHandle = Handle;
	ActorInfo = *OwnerInfo;

	TryPlayMontageTask();
}

bool UGameplayAbility_Anim::TryPlayMontageTask()
{
	auto AbilitySystemComponent = ActorInfo.AbilitySystemComponent.Get();
	auto AnimInstance = ActorInfo.GetAnimInstance();

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
				FGameplayEffectContextHandle EffectContext = MakeEffectContext(AbilitySpecHandle, &ActorInfo);
				FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect, 1.f, EffectContext);
				if (EffectHandle.IsValid())
				{
					AppliedEffects.Add(EffectHandle);
				}
			}
		}

		// 启动动画任务
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, MontageToPlay, PlayRate, SectionName, false, 1.0f, 0.f);
		MontageTask->OnCancelled.AddDynamic(this, &UGameplayAbility_Anim::OnCancelled);
		MontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_Anim::OnInterrupted);
		MontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_Anim::OnBlendOut);
		MontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_Anim::OnCompleted);
		MontageTask->Activate();
		return true;
	}
	return false;
}

void UGameplayAbility_Anim::OnTaskEnd()
{
	auto ASC = ActorInfo.AbilitySystemComponent.Get();
	if (ASC != nullptr)
	{
		for (FActiveGameplayEffectHandle Handle : AppliedEffects)
		{
			ASC->RemoveActiveGameplayEffect(Handle);
		}
	}
}

float UGameplayAbility_Anim::GetProgress()
{
	return 0.f;
}

void UGameplayAbility_Anim::OnCancelled()
{
	OnTaskEnd();

	K2_EndAbility();
}

void UGameplayAbility_Anim::OnInterrupted()
{
	OnTaskEnd();

	K2_EndAbility();
}

void UGameplayAbility_Anim::OnBlendOut()
{
	OnTaskEnd();
}

void UGameplayAbility_Anim::OnCompleted()
{
	K2_EndAbility();
}