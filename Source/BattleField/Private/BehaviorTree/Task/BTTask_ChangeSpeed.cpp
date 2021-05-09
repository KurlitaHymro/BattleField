// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTask_ChangeSpeed.h"
#include "Character/BattlefieldCharacterBase.h"
#include "AIController.h"

UBTTask_ChangeSpeed::UBTTask_ChangeSpeed()
{
	NodeName = "ChangeSpeed";
}

EBTNodeResult::Type UBTTask_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();

	if (OwnerController) {
		ABattlefieldCharacterBase* self = Cast<ABattlefieldCharacterBase>(OwnerController->GetPawn());
		if (self) {
			self->GetState()->SpeedChange(SpeedChangValue);
		}
	}
	return EBTNodeResult::Succeeded;
}