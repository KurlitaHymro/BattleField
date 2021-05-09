// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/BTService_GetRandReachablePoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Common/CommonInterface.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTService_GetRandReachablePoint::UBTService_GetRandReachablePoint(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	NodeName = "RandomReachablePoint";

	bNotifyTick = true;
	bTickIntervals = false;
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bCreateNodeInstance = true;
	
	// accept only actors and vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_GetRandReachablePoint, BlackboardKey));
}

void UBTService_GetRandReachablePoint::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

}

void UBTService_GetRandReachablePoint::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	
}

void UBTService_GetRandReachablePoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (OwnerController && OwnerController) {
		FNavLocation targetLocation;
		UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomReachablePointInRadius(
			OwnerController->GetPawn()->GetActorLocation(),
			PatrolRadius,
			targetLocation);
		MyBlackboard->SetValueAsVector(BlackboardKey.SelectedKeyName, targetLocation.Location);
	}
}

void UBTService_GetRandReachablePoint::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

}