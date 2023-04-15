// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CalculateDistance.generated.h"

struct FBTDistanceMemory
{
	AActor* TargetActorSet;
	FVector TargetLocationSet;
	bool bActorSet;

	void Reset()
	{
		TargetActorSet = nullptr;
		TargetLocationSet = FAISystem::InvalidLocation;
		bActorSet = false;
	}
};

/**
 * 
 */
UCLASS()
class BATTLEAIRUNTIME_API UBTService_CalculateDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector DistanceKey;

protected:
	UBTService_CalculateDistance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTDistanceMemory); }
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

};
