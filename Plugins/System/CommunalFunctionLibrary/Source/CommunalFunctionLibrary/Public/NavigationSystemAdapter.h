// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NavigationSystemAdapter.generated.h"

/**
 * 
 */
UCLASS()
class COMMUNALFUNCTIONLIBRARY_API UNavigationSystemAdapter : public UObject
{
	GENERATED_BODY()

private:
	UNavigationSystemAdapter(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void BeginDestroy() override;

private:
	static UNavigationSystemAdapter* Instance;

	class UNavigationSystemV1* NS;

public:
	UFUNCTION()
	static UNavigationSystemAdapter* GetInstance();

	UFUNCTION()
	FVector GetRandomLocationWithNavigationSystem(UWorld* World, FVector CenterLocation, float RandomRadius);
};
