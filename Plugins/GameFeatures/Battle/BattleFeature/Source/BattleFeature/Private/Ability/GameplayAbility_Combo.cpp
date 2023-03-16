// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayAbility_Combo.h"
#include "AbilitiesInputComponent.h"
#include "AbilitySystemComponent.h"
#include "GrantAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_Combo)

void UGameplayAbility_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, OwnerInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateAbility Failed (Ability not be commited)"));
		return;
	}

	AbilitySpecHandle = Handle;
	ActorInfo = *OwnerInfo;
	bPlayComboMontageOnBlendOut = false;

	auto ASC = ActorInfo.AbilitySystemComponent.Get();
	if (!ASC->HasMatchingGameplayTag(DuringMontageTag))
	{
		TryPlayMontageTask();
	}
	else
	{
		if (ASC->HasMatchingGameplayTag(ComboInputTag))
		{
			// 启动混出等待
			UAbilityTask_WaitGameplayTagRemoved* WaitMontageEndTask = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(
				this, DuringMontageTag, nullptr, false);
			WaitMontageEndTask->Removed.AddDynamic(this, &UGameplayAbility_Combo::OnMontageEnd);
			WaitMontageEndTask->Activate();

			// 提前结束输入窗口并启用立即混出
			ASC->RemoveActiveGameplayEffectBySourceEffect(ComboInputEffect, nullptr);
			bPlayComboMontageOnBlendOut = true;
		}
		else
		{
			K2_EndAbility();
		}
	}
}

void UGameplayAbility_Combo::OnMontageEnd()
{
	if (!TryPlayMontageTask())
	{
		FDateTime now = FDateTime::Now();
		UE_LOG(LogTemp, Warning, TEXT("%d.%d FailPlayMontage %s"), now.GetSecond(), now.GetMillisecond(), *MontageToPlay.GetName());
		K2_EndAbility();
	}
}

void UGameplayAbility_Combo::OnComboInputStart()
{
	FDateTime now = FDateTime::Now();
	UE_LOG(LogTemp, Warning, TEXT("%d.%d ComboInputStart %s"), now.GetSecond(), now.GetMillisecond(), *MontageToPlay.GetName());
	UpdateDervieAbilities();
}

void UGameplayAbility_Combo::OnBlendOut()
{
	FDateTime now = FDateTime::Now();
	UE_LOG(LogTemp, Warning, TEXT("%d.%d BlendOut %s"), now.GetSecond(), now.GetMillisecond(), *MontageToPlay.GetName());
	OnTaskEnd();

	auto ASC = ActorInfo.AbilitySystemComponent.Get();
	ASC->RemoveActiveGameplayEffectBySourceEffect(DuringMontageEffect, nullptr);
	if (bPlayComboMontageOnBlendOut)
	{
		MontageStop();
	}
	else
	{
		UGrantAbilitySystemComponent* GASC = Cast<UGrantAbilitySystemComponent>(ActorInfo.AbilitySystemComponent.Get());
		TWeakObjectPtr<AActor> AvatarActor = ActorInfo.AvatarActor.IsValid() ? ActorInfo.AvatarActor : ActorInfo.OwnerActor;
		auto AIC = Cast<UAbilitiesInputComponent>(AvatarActor->GetComponentByClass(UAbilitiesInputComponent::StaticClass()));

		if (GASC == nullptr || AIC == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent or AbilitiesInputComponent is null"));
			return;
		}

		// 还原技能
		for (auto Dervie : DervieAbilities)
		{
			FGameplayAbilitySpecHandle* Handle = GASC->GrantedAbilitiesHandle.Find(Dervie.Value);
			if (Handle != nullptr && Handle->IsValid())
			{
				AIC->TeardownAbilityBinding(*Handle);
			}
		}
	}
}

void UGameplayAbility_Combo::OnCompleted()
{
	FDateTime now = FDateTime::Now();
	UE_LOG(LogTemp, Warning, TEXT("%d.%d Completed %s"), now.GetSecond(), now.GetMillisecond(), *MontageToPlay.GetName());
	K2_EndAbility();
}

bool UGameplayAbility_Combo::TryPlayMontageTask()
{
	bool bSuccess = Super::TryPlayMontageTask();
	if (bSuccess)
	{
		FDateTime now = FDateTime::Now();
		UE_LOG(LogTemp, Warning, TEXT("%d.%d SuccPlayMontage %s"), now.GetSecond(), now.GetMillisecond(), *MontageToPlay.GetName());
		// 启动连击等待
		UAbilityTask_WaitGameplayTagAdded* WaitComboInputStartTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(
			this, ComboInputTag, nullptr, false);
		WaitComboInputStartTask->Added.AddDynamic(this, &UGameplayAbility_Combo::OnComboInputStart);
		WaitComboInputStartTask->Activate();

		// 施加硬直效果
		auto ASC = ActorInfo.AbilitySystemComponent.Get();
		ASC->ApplyGameplayEffectToSelf(DuringMontageEffect->GetDefaultObject<UGameplayEffect>(), 0.f, ASC->MakeEffectContext());

		return true;
	}
	return false;
}

void UGameplayAbility_Combo::UpdateDervieAbilities()
{
	UGrantAbilitySystemComponent* GASC = Cast<UGrantAbilitySystemComponent>(ActorInfo.AbilitySystemComponent.Get());
	TWeakObjectPtr<AActor> AvatarActor = ActorInfo.AvatarActor.IsValid() ? ActorInfo.AvatarActor : ActorInfo.OwnerActor;
	auto AIC = Cast<UAbilitiesInputComponent>(AvatarActor->GetComponentByClass(UAbilitiesInputComponent::StaticClass()));
	
	if (GASC == nullptr || AIC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent or AbilitiesInputComponent is null"));
		return;
	}

	// 成功释放一个技能后，将另一个技能绑定到操作栈上。
	for (auto Dervie : DervieAbilities)
	{
		FGameplayAbilitySpecHandle* Handle = GASC->GrantedAbilitiesHandle.Find(Dervie.Value);
		if (Handle != nullptr && Handle->IsValid())
		{
			AIC->SetupBinding(Dervie.Key, *Handle);
		}
	}
}

void UGameplayAbility_Combo::TryTriggerDervieAbility()
{

}

