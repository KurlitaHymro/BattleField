// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SelectEnemyActor.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API UBTTask_SelectEnemyActor : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	UBTTask_SelectEnemyActor();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
