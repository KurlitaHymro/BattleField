// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonControlsComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UThirdPersonControlsComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	BindInputAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveForward);
	BindInputAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveRight);
	BindInputAction(LookUpInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLookUp);
	BindInputAction(TurnInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleTurn);
	BindInputAction(JumpInputAction, ETriggerEvent::Started, this, &ThisClass::HandleStartJump);
	BindInputAction(JumpInputAction, ETriggerEvent::Completed, this, &ThisClass::HandleStopJump);
	BindInputAction(RunInputAction, ETriggerEvent::Started, this, &ThisClass::HandleStartRun);
	BindInputAction(RunInputAction, ETriggerEvent::Completed, this, &ThisClass::HandleStopRun);
	BindInputAction(InteractInputAction, ETriggerEvent::Started, this, &ThisClass::HandleInteract);

	BaseLookUpRate = 60.f;
	BaseTurnRate = 60.f;
	SpeedChangeValue = 200.f;
}

void UThirdPersonControlsComponent::HandleMoveForward(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();
	float Val = InputActionValue.Get<FInputActionValue::Axis1D>();
	
	if (MyPawn->Controller == nullptr || Val == 0.0f)
	{
		return;
	}
	MoveForwardActionValue = Val;

	// find out which way is right
	const FRotator Rotation = MyPawn->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// add movement in that direction
	MyPawn->AddMovementInput(Direction, BlendMovementVector().X);
}

void UThirdPersonControlsComponent::HandleMoveRight(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();
	float Val = InputActionValue.Get<FInputActionValue::Axis1D>();

	if (MyPawn->Controller == nullptr || Val == 0.0f)
	{
		return;
	}
	MoveRightActionValue = Val;

	// find out which way is right
	const FRotator Rotation = MyPawn->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	MyPawn->AddMovementInput(Direction, BlendMovementVector().Y);
}

void UThirdPersonControlsComponent::HandleLookUp(const FInputActionValue& InputActionValue)
{	
	APawn* MyPawn = GetPawnChecked<APawn>();
	float Val = InputActionValue.Get<FInputActionValue::Axis1D>();

	if (MyPawn->Controller == nullptr || Val == 0.0f)
	{
		return;
	}

	// calculate delta for this frame from the rate information
	MyPawn->AddControllerPitchInput(Val * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void UThirdPersonControlsComponent::HandleTurn(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();
	float Val = InputActionValue.Get<FInputActionValue::Axis1D>();

	if (MyPawn->Controller == nullptr || Val == 0.0f)
	{
		return;
	}

	// calculate delta for this frame from the rate information
	MyPawn->AddControllerYawInput(Val * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void UThirdPersonControlsComponent::HandleStartJump(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();

	ACharacter* MyCharacter = CastChecked<ACharacter>(MyPawn);

	MyCharacter->Jump();
}

void UThirdPersonControlsComponent::HandleStopJump(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();

	ACharacter* MyCharacter = CastChecked<ACharacter>(MyPawn);

	MyCharacter->StopJumping();
}

void UThirdPersonControlsComponent::HandleStartRun(const FInputActionValue& InputActionValue)
{
	if (SpeedChangeValue < 0.f)
	{
		return;
	}

	ActualSpeedChangeValue = ChangeSpeed(SpeedChangeValue);
}

void UThirdPersonControlsComponent::HandleStopRun(const FInputActionValue& InputActionValue)
{
	ChangeSpeed(-ActualSpeedChangeValue);
	ActualSpeedChangeValue = 0.f;
}

void UThirdPersonControlsComponent::HandleInteract(const FInputActionValue& InputActionValue)
{
	
}

FVector UThirdPersonControlsComponent::BlendMovementVector()
{
	FVector inputVec = UKismetMathLibrary::MakeVector(MoveForwardActionValue, MoveRightActionValue, 0.f);
	if (inputVec.Size() < 0.586f)
	{
		inputVec = UKismetMathLibrary::ClampVectorSize(inputVec, 0.287f, 0.332f);
	}
	else
	{
		inputVec = UKismetMathLibrary::ClampVectorSize(inputVec, 0.625f, 1.000f);
	}
	return inputVec;
}

float UThirdPersonControlsComponent::ChangeSpeed(float Val)
{
	APawn* MyPawn = GetPawnChecked<APawn>();
	ACharacter* MyCharacter = CastChecked<ACharacter>(MyPawn);

	float Speed = MyCharacter->GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = Speed + Val;

	if (NewSpeed < 0.f) {
		NewSpeed = 0.f;
	}

	MyCharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	return NewSpeed - Speed;
}