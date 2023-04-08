// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddToWorldBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BATTLEFIELD_API UGameFeatureAction_AddToWorldBase : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~ End UGameFeatureAction interface

private:
	void HandleGameInstanceStart(UGameInstance* GameInstance);
	virtual void AddToWorld(const FWorldContext& WorldContext) PURE_VIRTUAL(UGameFeatureAction_WorldActionBase::AddToWorld, );

private:
	FDelegateHandle GameInstanceStartHandle;
};
