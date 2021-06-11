// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddSpawnedActors.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

void UGameFeatureAction_AddSpawnedActors::OnGameFeatureActivating()
{
	GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddSpawnedActors::HandleGameInstanceStart);

	// Add to any worlds with associated game instances that have already been initialized
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		AddToWorld(WorldContext);
	}
}

void UGameFeatureAction_AddSpawnedActors::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FWorldDelegates::OnStartGameInstance.Remove(GameInstanceStartHandle);
}

void UGameFeatureAction_AddSpawnedActors::HandleGameInstanceStart(UGameInstance* GameInstance)
{
	if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
	{
		AddToWorld(*WorldContext);
	}
}
