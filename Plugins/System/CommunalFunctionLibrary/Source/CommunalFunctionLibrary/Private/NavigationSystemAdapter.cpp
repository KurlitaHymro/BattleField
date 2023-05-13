// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationSystemAdapter.h"
#include "NavigationSystem.h"

UNavigationSystemAdapter* UNavigationSystemAdapter::Instance = nullptr;

UNavigationSystemAdapter::UNavigationSystemAdapter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UNavigationSystemAdapter::BeginDestroy()
{
	Super::BeginDestroy();

	Instance = nullptr;
}

UNavigationSystemAdapter* UNavigationSystemAdapter::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UNavigationSystemAdapter>();
	}
	return Instance;
}

FVector UNavigationSystemAdapter::GetRandomLocationWithNavigationSystem(UWorld* World, FVector CenterLocation, float RandomRadius)
{
	NS = UNavigationSystemV1::GetCurrent(World);
	FVector TargetLocation = CenterLocation;

	if (NS != nullptr && RandomRadius > 0)
	{
		FNavLocation NavLocation;
		if (NS->GetRandomReachablePointInRadius(TargetLocation, RandomRadius, NavLocation))
		{
			TargetLocation = NavLocation.Location;
		}
	}
	return TargetLocation;
}
