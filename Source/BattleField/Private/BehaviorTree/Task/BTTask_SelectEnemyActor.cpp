// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTask_SelectEnemyActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/AIControllerBase.h"
#include "Common/CommonInterface.h"

UBTTask_SelectEnemyActor::UBTTask_SelectEnemyActor()
{
	NodeName = "SelectEnemy";
}

EBTNodeResult::Type UBTTask_SelectEnemyActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	AAIControllerBase* MyAIController = Cast<AAIControllerBase>(OwnerController);
	if (MyAIController) {
		if (MyAIController->TargetEnemy.Num() > 0) {
			MyBlackboard->SetValueAsObject(BlackboardKey.SelectedKeyName, MyAIController->TargetEnemy[0]);
			return EBTNodeResult::Succeeded;
		} else {
			MyBlackboard->SetValueAsObject(BlackboardKey.SelectedKeyName, nullptr);
			return EBTNodeResult::Succeeded;
		}
	} else {
		return EBTNodeResult::Failed;
	}
}