// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BattlefieldCharacterBase.h"
#include "BattlefieldCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API ABattlefieldCharacterPlayer : public ABattlefieldCharacterBase
{
	GENERATED_BODY()

	class USpringArmComponent* SpringArm;

	class UCameraComponent* Camera;

	class UUserWidget* StateWidget;

public:
	// Sets default values for this character's properties
	ABattlefieldCharacterPlayer();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CharacterStateUpdate_Implementation(EnumActorStateItem state) override;

	virtual void CreateStateWidget_Implementation() override;
};
