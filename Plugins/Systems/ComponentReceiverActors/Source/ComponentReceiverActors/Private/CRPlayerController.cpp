// Fill out your copyright notice in the Description page of Project Settings.


#include "CRPlayerController.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/ControllerComponent.h"

void ACRPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ACRPlayerController::ReceivedPlayer()
{
	// Player controllers always get assigned a player and can't do much until then
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::ReceivedPlayer();

	TArray<UControllerComponent*> CRComponents;
	GetComponents(CRComponents);
	for (UControllerComponent* Component : CRComponents)
	{
		Component->ReceivedPlayer();
	}
}

void ACRPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

void ACRPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TArray<UControllerComponent*> CRComponents;
	GetComponents(CRComponents);
	for (UControllerComponent* Component : CRComponents)
	{
		Component->PlayerTick(DeltaTime);
	}
}