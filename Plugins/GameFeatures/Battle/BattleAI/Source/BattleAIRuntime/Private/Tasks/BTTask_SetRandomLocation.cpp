// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTTask_SetRandomLocation.h"
#include "NavigationSystemAdapter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_SetRandomLocation)

UBTTask_SetRandomLocation::UBTTask_SetRandomLocation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Random ReachableLocation";

	Radius = 0.f;

	// accept only actors and vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UBTTask_SetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (OwnerController && MyBlackboard)
	{
		auto TargetLocation = UNavigationSystemAdapter::GetInstance()->GetRandomLocationWithNavigationSystem(GetWorld(), OwnerController->GetPawn()->GetActorLocation(), Radius);
		MyBlackboard->SetValueAsVector(BlackboardKey.SelectedKeyName, TargetLocation);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}


FString UBTTask_SetRandomLocation::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDesc);
}

void UBTTask_SetRandomLocation::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	auto KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	Values.Add(FString::Printf(TEXT("move target: %s"), *KeyDesc));
}

#if WITH_EDITOR

FName UBTTask_SetRandomLocation::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.MoveTo.Icon");
}

void UBTTask_SetRandomLocation::OnNodeCreated()
{
	
}

#endif	// WITH_EDITOR