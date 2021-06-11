// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddInputMapping.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

void UGameFeatureAction_AddInputMapping::OnGameFeatureActivating()
{
	GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddInputMapping::HandleGameInstanceStart);

	// Add to any worlds with associated game instances that have already been initialized
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		AddToWorld(WorldContext);
	}
}

void UGameFeatureAction_AddInputMapping::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FWorldDelegates::OnStartGameInstance.Remove(GameInstanceStartHandle);
}

void UGameFeatureAction_AddInputMapping::HandleGameInstanceStart(UGameInstance* GameInstance)
{
	if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
	{
		AddToWorld(*WorldContext);
	}
}
