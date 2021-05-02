// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ActorState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UActorState::UActorState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorState::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UActorState::GetStateItem(EnumActorStateItem stateItem)
{
	switch (stateItem) {
	case EnumActorStateItem::EN_HP:
		return HP;
	case EnumActorStateItem::EN_MP:
		return MP;
	case EnumActorStateItem::EN_ATK:
		return ATK;
	case EnumActorStateItem::EN_DEF:
		return DEF;
	case EnumActorStateItem::EN_SPD:
		return SPD;
	default:
		UE_LOG(RunLog, Warning, TEXT("Invalid State Item"));
		return 0;
	}
}

float UActorState::GetHpPercent()
{
	return static_cast<float>(HP) / static_cast<float>(maxHP);
}

void UActorState::InitCharacterStateByInfo(FPropertyTableRow& cInfo)
{
	int hp = cInfo.Rank * 100 + cInfo.Level * 10;
	int mp = cInfo.Level * 10;
	int atk = cInfo.Level * 5;
	int def = cInfo.Level * 3;
	int spd = 450;
	InitCharacterState(hp, mp, atk, def, spd);
}

void UActorState::InitCharacterState(int hp, int mp, int atk, int def, int spd)
{
	maxHP = hp;
	maxMP = mp;
	baseATK = atk;
	baseDEF = def;
	baseSPD = spd;

	HP = hp;
	MP = mp;
	ATK = atk;
	DEF = def;
	SPD = spd;

	HpChange(0);
	SpeedChange(0);
}

void UActorState::SetCharacterState(EnumActorStateItem stateItem, int newState)
{
	switch (stateItem) {
	case EnumActorStateItem::EN_HP:
		HpChange(newState - HP);
		return;
	case EnumActorStateItem::EN_MP:
		MP = newState;
		return;
	case EnumActorStateItem::EN_ATK:
		ATK = newState;
		return;
	case EnumActorStateItem::EN_DEF:
		DEF = newState;
		return;
	case EnumActorStateItem::EN_SPD:
		SpeedChange(newState - SPD);
		return;
	default:
		return;
	}
}

void UActorState::HpChange(int changeValue)
{
	AActor* owner = Cast<AActor>(GetOwner());
	if (owner) {
		if (HP + changeValue > maxHP) {
			HP = maxHP;
		} else if (HP + changeValue <= 0) {
			HP = 0;
		} else {
			HP = HP + changeValue;
		}
	} else {
		UE_LOG(RunLog, Error, TEXT("Invalid Component Owner"));
	}
}

void UActorState::SpeedChange(int changeValue)
{
	if (SPD + changeValue <= 0) {
		SPD = 0;
	} else {
		SPD = SPD + changeValue;
	}

	// Character持有本组件时，速度变化后，更新移动组件。
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner) {
		owner->GetCharacterMovement()->MaxWalkSpeed = static_cast<float>(SPD);
	} else {
		UE_LOG(RunLog, Error, TEXT("Invalid Component Owner"));
	}
}

void UActorState::ResetSpeed()
{
	SpeedChange(baseSPD - SPD);
}