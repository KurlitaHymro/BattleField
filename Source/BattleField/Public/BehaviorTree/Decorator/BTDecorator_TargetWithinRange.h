// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_TargetWithinRange.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API UBTDecorator_TargetWithinRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = Behavior, EditAnywhere, BlueprintReadWrite)
	float RangeDistance = 0.f;

	UPROPERTY(Category = Behavior, EditAnywhere, BlueprintReadWrite)
	float AngleDegrees = 0.f;

	UBTDecorator_TargetWithinRange();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
