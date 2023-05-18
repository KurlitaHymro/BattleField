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

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttackAttributeSet, ShockLevel)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(ShockLevel)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(ShockLevel)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(ShockLevel)

private:
	friend struct BattleDamageStatics;

	// ¹¥»÷Á¦
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;

	// ÎäÆ÷»ÓÎè¹¥»÷Ö¡ÖÐµÄ¸½¼ÓÕÐÊ½ÉËº¦ÏµÊý
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MeleeMoveFactor;

	// ÎäÆ÷»ÓÎè¹¥»÷Ö¡ÖÐµÄ¸½¼Ó³å»÷Ç¿¶È
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShockLevel;
};