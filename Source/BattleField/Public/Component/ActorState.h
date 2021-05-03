// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/CharacterProperty.h"
#include "ActorState.generated.h"

UENUM(BlueprintType)
enum class EnumActorStateItem : uint8
{
	EN_HP  UMETA(DisplayName = "HP"),
	EN_MP  UMETA(DisplayName = "MP"),
	EN_ATK UMETA(DisplayName = "ATK"),
	EN_DEF UMETA(DisplayName = "DEF"),
	EN_SPD UMETA(DisplayName = "WalkSpeed"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEFIELD_API UActorState : public UActorComponent
{
	GENERATED_BODY()

	int maxHP;

	int HP;

	int maxMP;

	int MP;

	int baseATK;

	int ATK;

	int baseDEF;

	int DEF;

	int baseSPD;

	int SPD;
public:	
	// Sets default values for this component's properties
	UActorState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Category = "State", BlueprintCallable)
	int GetStateItem(EnumActorStateItem stateItem);

	UFUNCTION(Category = "State", BlueprintCallable)
	float GetHpPercent();

	UFUNCTION(Category = "State", BlueprintCallable)
	void InitCharacterStateByInfo(struct FPropertyTableRow& cInfo);

	UFUNCTION(Category = "State", BlueprintCallable)
	void InitCharacterState(int hp, int mp, int atk, int def, int spd);

	UFUNCTION(Category = "State", BlueprintCallable)
	void SetCharacterState(EnumActorStateItem stateItem, int newState);

	UFUNCTION(Category = "State", BlueprintCallable)
	void HpChange(int changeValue);

	UFUNCTION(Category = "State", BlueprintCallable)
	void SpeedChange(int changeValue);

	UFUNCTION(Category = "State", BlueprintCallable)
	void ResetSpeed();
};
