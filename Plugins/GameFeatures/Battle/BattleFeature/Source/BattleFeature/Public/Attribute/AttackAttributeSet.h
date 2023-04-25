// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttackAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UAttackAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttackAttributeSet, AttackPower)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(AttackPower)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(AttackPower)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(AttackPower)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttackAttributeSet, MeleeMoveFactor)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MeleeMoveFactor)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MeleeMoveFactor)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MeleeMoveFactor)

private:
	friend struct BattleDamageStatics;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;

	// 武器挥舞命中时，玩家当前的招式伤害系数。
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MeleeMoveFactor;
};