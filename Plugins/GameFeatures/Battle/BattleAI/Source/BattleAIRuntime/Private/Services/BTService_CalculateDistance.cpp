// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/BTService_CalculateDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTService_CalculateDistance::UBTService_CalculateDistance(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Calculate Distance";

	bTickIntervals = true;
	Interval = 1.0f;
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	// accept only actors and vectors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CalculateDistance, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CalculateDistance, BlackboardKey));
}

void UBTService_CalculateDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	FBTDistanceMemory* MyMemory = CastInstanceNodeMemory<FBTDistanceMemory>(NodeMemory);
	check(MyMemory);
	MyMemory->Reset();

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (OwnerController != nullptr && MyBlackboard != nullptr)
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
			AActor* TargetActor = Cast<AActor>(KeyValue);
			if (TargetActor)
			{
				MyMemory->TargetActorSet = TargetActor;
				MyMemory->bActorSet = true;
			}
		}
		else
		{
			const FVector TargetLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
			MyMemory->TargetLocationSet = TargetLocation;
		}
	}
}

void UBTService_CalculateDistance::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
}

void UBTService_CalculateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FBTDistanceMemory* MyMemory = CastInstanceNodeMemory<FBTDistanceMemory>(NodeMemory);
	check(MyMemory);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (OwnerController != nullptr && MyBlackboard != nullptr)
	{
		float Distance;
		if (MyMemory->bActorSet)
		{
			Distance = OwnerController->GetPawn()->GetDistanceTo(MyMemory->TargetActorSet);
		}
		else
		{
			Distance = (OwnerController->GetPawn()->GetActorLocation() - MyMemory->TargetLocationSet).Size();
		}
		MyBlackboard->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);
	}
}

FString UBTService_CalculateDistance::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() ||
		BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("Calculate Distance to %s"), *KeyDesc);
}

#if WITH_EDITOR
FName UBTService_CalculateDistance::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Service.DefaultFocus.Icon");
}
#endif	// WITH_EDITOR