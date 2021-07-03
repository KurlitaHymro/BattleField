// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMovementInputComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterMovementInputComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	BindInputAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMove);
	BindInputAction(SightInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleSight);
	BindInputAction(JumpInputAction, ETriggerEvent::Started, this, &ThisClass::HandleStartJump);
	BindInputAction(JumpInputAction, ETriggerEvent::Completed, this, &ThisClass::HandleStopJump);
	BindInputAction(RunInputAction, ETriggerEvent::Started, this, &ThisClass::HandleStartRun);
	BindInputAction(RunInputAction, ETriggerEvent::Completed, this, &ThisClass::HandleStopRun);
	BindInputAction(InteractInputAction, ETriggerEvent::Started, this, &ThisClass::HandleInteract);

	BaseLookUpRate = 60.f;
	BaseTurnRate = 60.f;
	SpeedChangeValue = 200.f;
}

void UCharacterMovementInputComponent::HandleMove(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();

	// 键盘输入为方形9个点，摇杆输入是圆环（径向死区0.2-1.0）。
	FVector Val = InputActionValue.Get<FInputActionValue::Axis3D>();

	if (MyPawn->Controller == nullptr || Val.IsZero())
	{
		return;
	}

	// 将移动输入Clamp到两个圆环中，对不同输入方式不会移动得过慢或过快。
	if (Val.Size() < 0.65f) {
		Val = UKismetMathLibrary::ClampVectorSize(Val, 0.3f, 0.5f);
	}
	else {
		Val = UKismetMathLibrary::ClampVectorSize(Val, 0.8f, 1.0f);
	}

	// find out which way is right
	const FRotator Rotation = MyPawn->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// add movement in that direction
	MyPawn->AddMovementInput(DirectionX, Val.X);
	MyPawn->AddMovementInput(DirectionY, Val.Y);
}

void UCharacterMovementInputComponent::HandleSight(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();

	// 鼠标输入为任意值，摇杆输入是圆环（径向死区0.2 - 1.0）。
	FVector2D Val = InputActionValue.Get<FInputActionValue::Axis2D>();

	if (MyPawn->Controller == nullptr || Val.IsZero())
	{
		return;
	}

	// find out which way is right
	const FRotator Rotation = MyPawn->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// add movement in that direction
	MyPawn->AddControllerPitchInput(Val.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	MyPawn->AddControllerYawInput(Val.X * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void UCharacterMovementInputComponent::HandleStartJump(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();

	ACharacter* MyCharacter = CastChecked<ACharacter>(MyPawn);

	MyCharacter->Jump();
}

void UCharacterMovementInputComponent::HandleStopJump(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();

	ACharacter* MyCharacter = CastChecked<ACharacter>(MyPawn);

	MyCharacter->StopJumping();
}

void UCharacterMovementInputComponent::HandleStartRun(const FInputActionValue& InputActionValue)
{
	if (SpeedChangeValue < 0.f)
	{
		return;
	}

	ActualSpeedChangeValue = ChangeSpeed(SpeedChangeValue);
}

void UCharacterMovementInputComponent::HandleStopRun(const FInputActionValue& InputActionValue)
{
	ChangeSpeed(-ActualSpeedChangeValue);
	ActualSpeedChangeValue = 0.f;
}

void UCharacterMovementInputComponent::HandleInteract(const FInputActionValue& InputActionValue)
{

}

float UCharacterMovementInputComponent::ChangeSpeed(float Val)
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