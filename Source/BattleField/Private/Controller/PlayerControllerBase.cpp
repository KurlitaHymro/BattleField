// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerControllerBase.h"

APlayerControllerBase::APlayerControllerBase()
{
	/* 填写输入-操作映射 */
	mAxisInput.Emplace(EnumControllerAxisInput::EN_AXIS0_Y, EnumCharacterAxisAction::EN_MOVE_Y);
	mAxisInput.Emplace(EnumControllerAxisInput::EN_AXIS0_X, EnumCharacterAxisAction::EN_MOVE_X);
	mAxisInput.Emplace(EnumControllerAxisInput::EN_AXIS1_Y, EnumCharacterAxisAction::EN_LOOK_Y);
	mAxisInput.Emplace(EnumControllerAxisInput::EN_AXIS1_X, EnumCharacterAxisAction::EN_LOOK_X);

	mOpInput.Emplace(EnumControllerOperatorInput::EN_OP1_PRS, EnumCharacterOnceAction::EN_SPEED_PLUS);
	mOpInput.Emplace(EnumControllerOperatorInput::EN_OP1_RLS, EnumCharacterOnceAction::EN_SPEED_SUB);
	mOpInput.Emplace(EnumControllerOperatorInput::EN_OP3_PRS, EnumCharacterOnceAction::EN_JUMP);
	mOpInput.Emplace(EnumControllerOperatorInput::EN_OP3_PRS, EnumCharacterOnceAction::EN_JUMP);
}

// Called when the game starts
void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

ABattlefieldCharacterBase* APlayerControllerBase::GetControlledPlayer()
{
	if (!CtrlPlayer) {
		CtrlPlayer = Cast<ABattlefieldCharacterBase>(GetPawn());
	}
	return CtrlPlayer;
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("Axis0X", this, &APlayerControllerBase::Axis0_X);
	InputComponent->BindAxis("Axis0Y", this, &APlayerControllerBase::Axis0_Y);
	InputComponent->BindAxis("Axis1X", this, &APlayerControllerBase::Axis1_X);
	InputComponent->BindAxis("Axis1Y", this, &APlayerControllerBase::Axis1_Y);

	InputComponent->BindAction("Op0", IE_Pressed,  this, &APlayerControllerBase::Op0_Prs);
	InputComponent->BindAction("Op0", IE_Released, this, &APlayerControllerBase::Op0_Rls);
	InputComponent->BindAction("Op1", IE_Pressed,  this, &APlayerControllerBase::Op1_Prs);
	InputComponent->BindAction("Op1", IE_Released, this, &APlayerControllerBase::Op1_Rls);
	InputComponent->BindAction("Op2", IE_Pressed,  this, &APlayerControllerBase::Op2_Prs);
	InputComponent->BindAction("Op2", IE_Released, this, &APlayerControllerBase::Op2_Rls);
	InputComponent->BindAction("Op3", IE_Pressed,  this, &APlayerControllerBase::Op3_Prs);
	InputComponent->BindAction("Op3", IE_Released, this, &APlayerControllerBase::Op3_Rls);
	InputComponent->BindAction("Op4", IE_Pressed,  this, &APlayerControllerBase::Op4_Prs);
	InputComponent->BindAction("Op4", IE_Released, this, &APlayerControllerBase::Op4_Rls);
	InputComponent->BindAction("Op5", IE_Pressed,  this, &APlayerControllerBase::Op5_Prs);
	InputComponent->BindAction("Op5", IE_Released, this, &APlayerControllerBase::Op5_Rls);
	InputComponent->BindAction("Op6", IE_Pressed,  this, &APlayerControllerBase::Op6_Prs);
	InputComponent->BindAction("Op6", IE_Released, this, &APlayerControllerBase::Op6_Rls);
	InputComponent->BindAction("Op7", IE_Pressed,  this, &APlayerControllerBase::Op7_Prs);
	InputComponent->BindAction("Op7", IE_Released, this, &APlayerControllerBase::Op7_Rls);
	InputComponent->BindAction("Op8", IE_Pressed,  this, &APlayerControllerBase::Op8_Prs);
	InputComponent->BindAction("Op8", IE_Released, this, &APlayerControllerBase::Op8_Rls);
	InputComponent->BindAction("Op9", IE_Pressed,  this, &APlayerControllerBase::Op9_Prs);
	InputComponent->BindAction("Op9", IE_Released, this, &APlayerControllerBase::Op9_Rls);
}

void APlayerControllerBase::Axis0_X(float v)
{
	if (v) {
		EnumCharacterAxisAction* action = mAxisInput.Find(EnumControllerAxisInput::EN_AXIS0_X);
		if (action) {
			GetControlledPlayer()->AxisInput(*action, v);
		}
	}
}

void APlayerControllerBase::Axis0_Y(float v)
{
	if (v) {
		EnumCharacterAxisAction* action = mAxisInput.Find(EnumControllerAxisInput::EN_AXIS0_Y);
		if (action) {
			GetControlledPlayer()->AxisInput(*action, v);
		}
	}
}

void APlayerControllerBase::Axis1_X(float v)
{
	if (v) {
		EnumCharacterAxisAction* action = mAxisInput.Find(EnumControllerAxisInput::EN_AXIS1_X);
		if (action) {
			GetControlledPlayer()->AxisInput(*action, v);
		}
	}
}

void APlayerControllerBase::Axis1_Y(float v)
{
	if (v) {
		EnumCharacterAxisAction* action = mAxisInput.Find(EnumControllerAxisInput::EN_AXIS1_Y);
		if (action) {
			GetControlledPlayer()->AxisInput(*action, v);
		}
	}
}

void APlayerControllerBase::Op0_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP0_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op0_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP0_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op1_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP1_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op1_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP1_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op2_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP2_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op2_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP2_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op3_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP3_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op3_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP3_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op4_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP4_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op4_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP4_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op5_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP5_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op5_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP5_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op6_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP6_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op6_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP6_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op7_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP7_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op7_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP7_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op8_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP8_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op8_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP8_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op9_Prs()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP9_PRS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}

void APlayerControllerBase::Op9_Rls()
{
	EnumCharacterOnceAction* action = mOpInput.Find(EnumControllerOperatorInput::EN_OP9_RLS);
	if (action) {
		GetControlledPlayer()->OnceInput(*action);
	}
}