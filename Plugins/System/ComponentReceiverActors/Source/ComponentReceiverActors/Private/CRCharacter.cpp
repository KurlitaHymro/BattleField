// Fill out your copyright notice in the Description page of Project Settings.


#include "CRCharacter.h"
#include "Components/GameFrameworkComponentManager.h"

// Sets default values
ACRCharacter::ACRCharacter()
{

}

// Called PreInitializeComponents
void ACRCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

// Called when the game starts or when spawned
void ACRCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

// Called when the game ends
void ACRCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
}

// Called every frame
void ACRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

