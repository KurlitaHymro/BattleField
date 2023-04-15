// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_DistanceJudge.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEAIRUNTIME_API UBTDecorator_DistanceJudge : public UBTDecorator_BlackboardBase
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Min Distance"))
	float MinDistance = 0.f;

	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Max Distance"))
	float MaxDistance = 0.f;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;
};
