// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CRGameState.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRECEIVERACTORS_API ACRGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void HandleMatchHasStarted() override;

};
