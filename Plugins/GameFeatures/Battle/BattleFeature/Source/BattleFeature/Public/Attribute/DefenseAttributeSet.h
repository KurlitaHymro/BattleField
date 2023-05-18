// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DefenseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UDefenseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDefenseAttributeSet, DefensePower)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(DefensePower)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(DefensePower)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(DefensePower)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDefenseAttributeSet, ApplyDamageFactor)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(ApplyDamageFactor)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(ApplyDamageFactor)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(ApplyDamageFactor)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDefenseAttributeSet, StoicLevel)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(StoicLevel)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(StoicLevel)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(StoicLevel)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDefenseAttributeSet, StableState)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(StableState)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(StableState)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(StableState)

private:
	friend struct BattleDamageStatics;

	// 防御力
	UPROPERTY(BlueprintReadOnly, Category = "Defense", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DefensePower;

	// 通用承伤系数
	UPROPERTY(BlueprintReadOnly, Category = "Defense", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ApplyDamageFactor;

	// 承受冲击时的忍耐强度
	UPROPERTY(BlueprintReadOnly, Category = "Defense", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StoicLevel;

	// 受到冲击后的瞬时稳定状态
	UPROPERTY(BlueprintReadOnly, Category = "Defense", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StableState;
};
