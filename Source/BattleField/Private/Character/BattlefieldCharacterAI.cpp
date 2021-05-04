// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattlefieldCharacterAI.h"
#include "Controller/AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"

// Sets default values
ABattlefieldCharacterAI::ABattlefieldCharacterAI()
{
	AutoPossessPlayer = EAutoReceiveInput::Type::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIControllerBase::StaticClass();
	BehaviorTree = LoadObject<UBehaviorTree>(NULL,
		TEXT("BehaviorTree'/Game/BehaviorTree/BehaviorTree.BehaviorTree'"));
	if (BehaviorTree) {
		
	} else {
		UE_LOG(LoadLog, Error, TEXT("Load BehaviorTree Error"));
	}
}