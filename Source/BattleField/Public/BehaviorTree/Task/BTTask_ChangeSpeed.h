// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API UBTTask_ChangeSpeed : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = Behavior, EditAnywhere, BlueprintReadWrite)
	float SpeedChangValue = 0.f;

	UBTTask_ChangeSpeed();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
