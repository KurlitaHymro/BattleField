// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMovementAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMoveSpeedRateAttribute())
	{
		float speed = MoveSpeed.GetBaseValue() * NewValue;
		SpeedSync(speed);
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Try To Change Speed Current Value %f"), OldValue);
	}
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
