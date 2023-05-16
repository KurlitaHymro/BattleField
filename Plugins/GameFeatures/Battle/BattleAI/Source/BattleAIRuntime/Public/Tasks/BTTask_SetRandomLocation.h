// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEAIRUNTIME_API UBTTask_SetRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(Category = Node, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Radius;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
	virtual void OnNodeCreated() override;
#endif // WITH_EDITOR

};
