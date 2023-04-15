// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MovementAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UMovementAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UMovementAttributeSet, MoveSpeed)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MoveSpeed)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MoveSpeed)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MoveSpeed)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UMovementAttributeSet, MoveSpeedRate)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MoveSpeedRate)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MoveSpeedRate)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MoveSpeedRate)

protected:
	virtual void InitFromMetaDataTable(const UDataTable* DataTable) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MoveSpeedRate;

	void SpeedSync(float NewSpeed);
};
