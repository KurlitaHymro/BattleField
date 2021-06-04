// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CRPlayerController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COMPONENTRECEIVERACTORS_API ACRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	virtual void ReceivedPlayer() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PlayerTick(float DeltaTime) override;

};
