// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattlefieldCharacterAI.h"
#include "Controller/AIControllerBase.h"

// Sets default values
ABattlefieldCharacterAI::ABattlefieldCharacterAI()
{
	AutoPossessPlayer = EAutoReceiveInput::Type::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIControllerBase::StaticClass();
}