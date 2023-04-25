// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/HealthAttributeSet.h"
#include "BattleCharacter.h"

void UHealthAttributeSet::InitFromMetaDataTable(const UDataTable* DataTable)
{
	Super::InitFromMetaDataTable(DataTable);

}

void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	
}

void UHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		if (NewValue <= 0.f)
		{
			ABattleCharacter* TargetCharacter = GetTypedOuter<ABattleCharacter>();
			TargetCharacter->Die();
		}
	}
}