// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommunalFunctionLibraryBPLibrary.h"
#include "CommunalFunctionLibrary.h"
#include "NavigationSystemAdapter.h"

UCommunalFunctionLibraryBPLibrary::UCommunalFunctionLibraryBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UCommunalFunctionLibraryBPLibrary::CommunalFunctionLibrarySampleFunction(float Param)
{
	return -1;
}

FVector UCommunalFunctionLibraryBPLibrary::GetRandomLocationWithNavigationSystem(UWorld* World, FVector CenterLocation, float RandomRadius)
{
	return UNavigationSystemAdapter::GetInstance()->GetRandomLocationWithNavigationSystem(World, CenterLocation, RandomRadius);
}

