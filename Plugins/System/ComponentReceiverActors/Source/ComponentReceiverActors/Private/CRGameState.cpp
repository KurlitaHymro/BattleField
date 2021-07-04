// Fill out your copyright notice in the Description page of Project Settings.


#include "CRGameState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/GameStateComponent.h"

void ACRGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ACRGameState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::BeginPlay();
}

void ACRGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

void ACRGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	TArray<UGameStateComponent*> CRComponents;
	GetComponents(CRComponents);
	for (UGameStateComponent* Component : CRComponents)
	{
		Component->HandleMatchHasStarted();
	}
}