// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CRAIController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COMPONENTRECEIVERACTORS_API ACRAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
