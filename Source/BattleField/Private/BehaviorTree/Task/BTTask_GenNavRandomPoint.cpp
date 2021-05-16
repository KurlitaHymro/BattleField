// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTask_GenNavRandomPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Common/CommonInterface.h"

UBTTask_GenNavRandomPoint::UBTTask_GenNavRandomPoint()
{
	NodeName = "RandomReachablePoint";
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GenNavRandomPoint, BlackboardKey));
}

EBTNodeResult::Type UBTTask_GenNavRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (OwnerController && MyBlackboard) {
		FNavLocation targetLocation;
		UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomReachablePointInRadius(
			OwnerController->GetPawn()->GetActorLocation(),
			PatrolRadius,
			targetLocation);
		MyBlackboard->SetValueAsVector(BlackboardKey.SelectedKeyName, targetLocation.Location);
		return EBTNodeResult::Succeeded;
	} else {
		UE_LOG(RunLog, Error, TEXT("Navigation RandomPoint Failed"));
		return EBTNodeResult::Failed;
	}
}
