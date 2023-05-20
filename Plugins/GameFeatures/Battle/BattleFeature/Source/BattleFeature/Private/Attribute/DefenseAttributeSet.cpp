// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DefenseAttributeSet.h"
#include "AbilitySystemGlobals.h"
void UDefenseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetStableStateAttribute())
	{
		if (NewValue < 0)
		{
			auto ASC = CastChecked<UAbilitySystemComponent>(GetOuter());
			if (ASC)
			{
				FGameplayEventData Payload;
				Payload.EventMagnitude = NewValue;
				ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Battle.Base.Trigger.UnderAttackDefault")), &Payload);
			}

		}
		StableState.SetCurrentValue(StableState.GetBaseValue());
	}
}