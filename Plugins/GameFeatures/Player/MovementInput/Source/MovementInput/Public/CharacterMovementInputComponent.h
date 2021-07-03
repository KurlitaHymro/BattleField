// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputModComponent.h"
#include "CharacterMovementInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTINPUT_API UCharacterMovementInputComponent : public UPawnInputModComponent
{
	GENERATED_BODY()

private:
	float ActualSpeedChangeValue;
	float ChangeSpeed(float Val);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Controls")
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Controls")
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Controls")
	float SpeedChangeValue;

protected:
	virtual void SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent) override;

	/** Movement input handlers */
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleSight(const FInputActionValue& InputActionValue);
	void HandleStartJump(const FInputActionValue& InputActionValue);
	void HandleStopJump(const FInputActionValue& InputActionValue);
	void HandleStartRun(const FInputActionValue& InputActionValue);
	void HandleStopRun(const FInputActionValue& InputActionValue);
	void HandleInteract(const FInputActionValue& InputActionValue);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* SightInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* RunInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* InteractInputAction;
};
