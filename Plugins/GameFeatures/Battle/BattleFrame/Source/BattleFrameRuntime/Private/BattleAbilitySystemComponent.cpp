// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAbilitySystemComponent.h"

namespace BattleAbilitySystemComponent_Impl
{
	constexpr int32 InvalidID = 0;
	int32 IncrementingID = InvalidID;

	static int32 GetNextID()
	{
		return ++IncrementingID;
	}
}

int32 UBattleAbilitySystemComponent::SynchronousLoadAbility(TSoftClassPtr<UGameplayAbility> AbilityType)
{
	FGameplayAbilitySpec AbilitySpec(AbilityType.LoadSynchronous());
	// 先试下先分再给有没有用
	int32 newID = BattleAbilitySystemComponent_Impl::GetNextID();
	AbilitySpec.InputID = newID;
	FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(AbilitySpec);

	FBattleAbilityEntry* Entry = &MappedAbilities.Add(newID);
	Entry->AbilityType = AbilityType;
	Entry->AbilitySpec = AbilitySpec;
	Entry->AbilitySpecHandle = AbilityHandle;

	return newID;
}

void UBattleAbilitySystemComponent::RemoveAbility(int32 AbilityID)
{
	FBattleAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		SetRemoveAbilityOnEnd(Ability->AbilitySpecHandle);
		MappedAbilities.Remove(AbilityID);
	}
}

void UBattleAbilitySystemComponent::EnableAbility(int32 AbilityID)
{
	FBattleAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		Ability->AbilitySpec.InputID = AbilityID;
	}
}

void UBattleAbilitySystemComponent::DisableAbility(int32 AbilityID)
{
	FBattleAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		Ability->AbilitySpec.InputID = BattleAbilitySystemComponent_Impl::InvalidID;
	}
}

FBattleAbilityEntry UBattleAbilitySystemComponent::FindAbilityInfo(int32 AbilityID)
{
	return FBattleAbilityEntry();
}

