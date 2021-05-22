// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattlefieldCharacterAI.h"
#include "Controller/AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/KismetMathLibrary.h"

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

	InMotionRotatorChangeRate = 2.0f;
}

FRotator ABattlefieldCharacterAI::GetExceptRotatorInMotion_Implementation()
{
	// 获取Controller中的Target
	if (!bIsValid) {
		return Super::GetExceptRotatorInMotion_Implementation();
	}
	AAIControllerBase* AIController = Cast<AAIControllerBase>(Controller);
	if (AIController) {
		AActor* Target = AIController->GetFirstPriorityTarget();
		if (Target) {
			return UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
		}
	} else {
		bIsValid = false;
	}
	return Super::GetExceptRotatorInMotion_Implementation();
}