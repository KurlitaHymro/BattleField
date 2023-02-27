// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CRCharacter.h"
#include "BfCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API ABfCharacter : public ACRCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABfCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called PreInitializeComponents
	virtual void PreInitializeComponents() override;

public:
	class UAbilitySystemComponent* ASC;
};
