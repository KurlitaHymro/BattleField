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
	// �������ȷ��ٸ���û����
	int32 newID = BattleAbilitySystemComponent_Impl::GetNextID();
	AbilitySpec.InputID = newID;
	FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(AbilitySpec);

	FBattleAbilityEntry Entry;
	Entry.AbilityType = AbilityType;
	Entry.AbilitySpec = AbilitySpec;
	Entry.AbilitySpecHandle = AbilityHandle;

	MappedAbilities.Add(newID, Entry);
	return 0;
}