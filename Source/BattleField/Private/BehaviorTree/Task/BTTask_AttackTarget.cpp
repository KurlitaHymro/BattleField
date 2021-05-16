// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTask_AttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BattlefieldCharacterBase.h"
#include "AIController.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
	NodeName = "Attack";
	bNotifyTick = true;
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_AttackTarget, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController) {
		CtrlPawn = Cast<ABattlefieldCharacterBase>(OwnerController->GetPawn());
	}

	MyBlackboard = OwnerComp.GetBlackboardComponent();

	return EBTNodeResult::InProgress;
}

void UBTTask_AttackTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (CtrlPawn && MyBlackboard) {
		AActor* target = Cast<AActor>(MyBlackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));
		if (target) {
			// TODO: 判断逻辑
			if (!CtrlPawn->bIsInMotion || CtrlPawn->bRcvNextInput) {
				CtrlPawn->MainNormalAttack();
				UE_LOG(RunLog, Warning, TEXT("Attack"));
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		} else {
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	} else {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}