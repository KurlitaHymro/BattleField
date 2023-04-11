// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AbilityOperator.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEAIRUNTIME_API UBTTask_AbilityOperator : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(Category = Node, EditAnywhere)
	TSoftClassPtr<class UGameplayAbility> AbilityType;

	UPROPERTY(Category = Node, EditAnywhere)
	uint32 bReverse : 1;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> MyOwnerComp;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
};
