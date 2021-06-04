// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CRPlayerState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COMPONENTRECEIVERACTORS_API ACRPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;

protected:
	virtual void CopyProperties(APlayerState* PlayerState);

};
