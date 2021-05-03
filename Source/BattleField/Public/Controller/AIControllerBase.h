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
	
	class UAIPerceptionComponent* perception;

	class UAISenseConfig_Sight* sight_cfg;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AAIControllerBase();

	UFUNCTION(Category = "Perception", BlueprintCallable)
	virtual void OnTargetPerceptionUpdated(AActor* actor, FAIStimulus stimulus);
};
