// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/Components/PawnControlsComponent.h"
#include "ThirdPersonControlsComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Category = "Input", meta = (BlueprintSpawnableComponent))
class THIRDPERSONCONTROLS_API UThirdPersonControlsComponent : public UPawnControlsComponent
{
	GENERATED_BODY()

	float MoveForwardActionValue;
	float MoveRightActionValue;
	float ActualSpeedChangeValue;
	FVector BlendMovementVector();
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
	void HandleMoveForward(const FInputActionValue& InputActionValue);
	void HandleMoveRight(const FInputActionValue& InputActionValue);
	void HandleLookUp(const FInputActionValue& InputActionValue);
	void HandleTurn(const FInputActionValue& InputActionValue);
	void HandleStartJump(const FInputActionValue& InputActionValue);
	void HandleStopJump(const FInputActionValue& InputActionValue);
	void HandleStartRun(const FInputActionValue& InputActionValue);
	void HandleStopRun(const FInputActionValue& InputActionValue);
	void HandleInteract(const FInputActionValue& InputActionValue);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* MoveForwardInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* MoveRightInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* LookUpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* TurnInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* RunInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controls")
	UInputAction* InteractInputAction;
};
