// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HealthAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UHealthAttributeSet, Hp)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Hp)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Hp)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Hp)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UHealthAttributeSet, MaxHp)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxHp)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxHp)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxHp)

protected:
	virtual void InitFromMetaDataTable(const UDataTable* DataTable) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Hp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHp;


};
