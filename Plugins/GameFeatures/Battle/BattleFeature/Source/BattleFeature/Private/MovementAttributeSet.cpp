// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMovementAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		if (ACharacter* TargetCharacter = GetTypedOuter<ACharacter>())
		{
			if (UCharacterMovementComponent* MovementComponent = TargetCharacter->GetCharacterMovement())
			{
				MovementComponent->MaxWalkSpeed = NewValue;
			}
		}
	}
}