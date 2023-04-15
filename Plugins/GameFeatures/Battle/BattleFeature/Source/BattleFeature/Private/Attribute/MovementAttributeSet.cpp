// Fill out your copyright notice in the Description page of Project Settings.

#include "Attribute/MovementAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMovementAttributeSet::InitFromMetaDataTable(const UDataTable* DataTable)
{
	Super::InitFromMetaDataTable(DataTable);

	SpeedSync(MoveSpeed.GetCurrentValue());
}

void UMovementAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	float NewSpeed = MoveSpeed.GetCurrentValue();
	if (Attribute == GetMoveSpeedRateAttribute())
	{
		NewSpeed += MoveSpeed.GetBaseValue() * (NewValue - OldValue);
		MoveSpeed.SetCurrentValue(NewSpeed);
	}
	SpeedSync(NewSpeed);
}

void UMovementAttributeSet::SpeedSync(float NewSpeed)
{
	if (ACharacter* TargetCharacter = GetTypedOuter<ACharacter>())
	{
		if (UCharacterMovementComponent* MovementComponent = TargetCharacter->GetCharacterMovement())
		{
			MovementComponent->MaxWalkSpeed = NewSpeed;
		}
	}
}