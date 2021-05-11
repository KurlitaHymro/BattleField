// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
	class UAIPerceptionComponent* PerceptionComp;

	class UAISenseConfig_Sight* sight_cfg;

	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorTreeComp;

	TArray<AActor*> TargetEnemy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(class APawn* InPawn) override;

	AAIControllerBase();

public:
	UFUNCTION(Category = "AI|Perception")
	virtual void OnTargetPerceptionUpdated(AActor* actor, FAIStimulus stimulus);

	UFUNCTION(Category = "AI|Perception")
	virtual void OnPawnDead();

	UFUNCTION(Category = "AI|Perception")
	virtual void UpdateBlackboard();
};
