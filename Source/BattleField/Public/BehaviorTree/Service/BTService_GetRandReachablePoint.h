// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_GetRandReachablePoint.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "随机可达点")
class BATTLEFIELD_API UBTService_GetRandReachablePoint : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = Behavior, EditAnywhere, BlueprintReadWrite)
	float PatrolRadius = 0.f;

	UBTService_GetRandReachablePoint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
};
