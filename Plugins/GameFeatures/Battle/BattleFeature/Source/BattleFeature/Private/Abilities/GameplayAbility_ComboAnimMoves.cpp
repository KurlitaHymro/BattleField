// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_ComboAnimMoves.h"
#include "AbilitySystemComponent.h"
#include "DamageSystem/AbilityTask_PlayMontageWaitEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

void UGameplayAbility_ComboAnimMoves::PlayDefaultAnimMoveMontage(FGameplayTagContainer EventTags, bool bStopWhenAbilityEnds)
{
	AnimMove = ComboAnimMoves[ComboCount];

	Super::PlayDefaultAnimMoveMontage(EventTags, bStopWhenAbilityEnds);

	EnableComboWait();
}

void UGameplayAbility_ComboAnimMoves::OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnBlendOut_Implementation(EventTag, EventData);

	ComboStartTask->EndTask();
	if (bComboEnable)
	{
		MontageTask->EndTask();
		ComboCount++;
		if (ComboCount < ComboAnimMoves.Num())
		{
			PlayDefaultAnimMoveMontage(FGameplayTagContainer(), false);
			bComboEnable = false;
		}
	}
}

void UGameplayAbility_ComboAnimMoves::EnableComboWait()
{
	auto ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	FGameplayTagContainer Container;
	Container.AddTag(CanComboTag);
	ASC->RemoveActiveEffectsWithTags(Container);

	ComboStartTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, CanComboTag, nullptr, true);
	ComboStartTask->Added.AddDynamic(this, &UGameplayAbility_ComboAnimMoves::OnCanComboTagAdd);
	ComboStartTask->Activate();
}

void UGameplayAbility_ComboAnimMoves::OnCanComboTagAdd_Implementation()
{
	auto WaitTryCombo = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
	WaitTryCombo->OnPress.AddDynamic(this, &UGameplayAbility_ComboAnimMoves::OnTryCombo);
	WaitTryCombo->Activate();
}

void UGameplayAbility_ComboAnimMoves::OnTryCombo_Implementation(float WaitTime)
{
	bComboEnable = true;
}
