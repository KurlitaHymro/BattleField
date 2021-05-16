// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GenNavRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API UBTTask_GenNavRandomPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = Behavior, EditAnywhere, BlueprintReadWrite)
	float PatrolRadius = 0.f;

	UBTTask_GenNavRandomPoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
