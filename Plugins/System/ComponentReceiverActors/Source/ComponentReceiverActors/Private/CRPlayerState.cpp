// Fill out your copyright notice in the Description page of Project Settings.


#include "CRPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/PlayerStateComponent.h"

void ACRPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ACRPlayerState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::BeginPlay();
}

void ACRPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

void ACRPlayerState::Reset()
{
	Super::Reset();

	TArray<UPlayerStateComponent*> CRComponents;
	GetComponents(CRComponents);
	for (UPlayerStateComponent* Component : CRComponents)
	{
		Component->Reset();
	}
}

void ACRPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::Reset();

	TArray<UPlayerStateComponent*> CRComponents;
	GetComponents(CRComponents);

	TArray<UPlayerStateComponent*> OtherCRComponents;
	PlayerState->GetComponents(OtherCRComponents);

	for (UPlayerStateComponent* Component : CRComponents)
	{
		for (UPlayerStateComponent* OtherComponent : OtherCRComponents)
		{
			Component->CopyProperties(OtherComponent);
		}
	}
}