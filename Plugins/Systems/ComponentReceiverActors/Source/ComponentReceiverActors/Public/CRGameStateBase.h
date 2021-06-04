// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CRGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTRECEIVERACTORS_API ACRGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
