// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAbilitySystemComponent.h"

void UBattleAbilitySystemComponent::ClearAddedAbilities()
{
	for (const auto Ability : AddedAbilityHandle)
	{
		SetRemoveAbilityOnEnd(Ability);
	}
	AddedAbilityHandle.Empty(DefaultAbilities.Num());
}

void UBattleAbilitySystemComponent::ClearAddedAttributes()
{
	for (const auto Attribute : AddedAttributeSet)
	{
		GetSpawnedAttributes_Mutable().Remove(Attribute);
	}
	AddedAttributeSet.Empty(DefaultAttributes.Num());
}

void UBattleAbilitySystemComponent::GrantDefaultAbilities()
{
	AddedAbilityHandle.Reserve(DefaultAbilities.Num());
	for (const auto &Ability : DefaultAbilities)
	{
		if (Ability.AbilityType)
		{
			AddedAbilityHandle.Add(GiveAbility(FGameplayAbilitySpec(Ability.AbilityType)));
		}
	}
}

void UBattleAbilitySystemComponent::GrantDefaultAttributes()
{
	AddedAttributeSet.Reserve(DefaultAttributes.Num());
	for (const auto& Attribute : DefaultAttributes)
	{
		if (Attribute.AttributeSetType) {
			UAttributeSet* AttributeSet = NewObject<UAttributeSet>(this, Attribute.AttributeSetType);
			if (Attribute.InitializationData)
			{
				AttributeSet->InitFromMetaDataTable(Attribute.InitializationData);
			}
			AddedAttributeSet.Add(AttributeSet);
			AddAttributeSetSubobject(AttributeSet);
		}
	}
}

void UBattleAbilitySystemComponent::GrantDefaultAbilitiesAndAttributes()
{
	ClearAddedAbilities();
	ClearAddedAttributes();
	GrantDefaultAbilities();
	GrantDefaultAttributes();
}
