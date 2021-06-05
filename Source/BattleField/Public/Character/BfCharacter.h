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
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Called for forwards/backward input */
	UFUNCTION(Category = "CharacterBehavior", BlueprintCallable)
	void MoveForward(float Value);

	/** Called for side to side input */
	UFUNCTION(Category = "CharacterBehavior", BlueprintCallable)
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(Category = "CharacterBehavior", BlueprintCallable)
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(Category = "CharacterBehavior", BlueprintCallable)
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	UFUNCTION(Category = "CharacterBehavior", BlueprintCallable)
	void JumpStarted();

	/** Handler for when a touch input stops. */
	UFUNCTION(Category = "CharacterBehavior", BlueprintCallable)
	void JumpStopped();
};
