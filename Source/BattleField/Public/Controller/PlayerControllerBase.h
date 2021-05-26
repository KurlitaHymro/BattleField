// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Character/BattlefieldCharacterPlayer.h"
#include "PlayerControllerBase.generated.h"

UENUM(BlueprintType)
enum class EnumControllerAxisInput : uint8
{
	EN_AXIS0_Y UMETA(DisplayName = "Axis0_Y"),
	EN_AXIS0_X UMETA(DisplayName = "Axis0_X"),
	EN_AXIS1_Y UMETA(DisplayName = "Axis1_Y"),
	EN_AXIS1_X UMETA(DisplayName = "Axis1_X"),
};

UENUM(BlueprintType)
enum class EnumControllerOperatorInput : uint8
{
	EN_OP0_PRS UMETA(DisplayName = "Op0_Prs"),
	EN_OP0_RLS UMETA(DisplayName = "Op0_Rls"),
	EN_OP1_PRS UMETA(DisplayName = "Op1_Prs"),
	EN_OP1_RLS UMETA(DisplayName = "Op1_Rls"),
	EN_OP2_PRS UMETA(DisplayName = "Op2_Prs"),
	EN_OP2_RLS UMETA(DisplayName = "Op2_Rls"),
	EN_OP3_PRS UMETA(DisplayName = "Op3_Prs"),
	EN_OP3_RLS UMETA(DisplayName = "Op3_Rls"),
	EN_OP4_PRS UMETA(DisplayName = "Op4_Prs"),
	EN_OP4_RLS UMETA(DisplayName = "Op4_Rls"),
	EN_OP5_PRS UMETA(DisplayName = "Op5_Prs"),
	EN_OP5_RLS UMETA(DisplayName = "Op5_Rls"),
	EN_OP6_PRS UMETA(DisplayName = "Op6_Prs"),
	EN_OP6_RLS UMETA(DisplayName = "Op6_Rls"),
	EN_OP7_PRS UMETA(DisplayName = "Op7_Prs"),
	EN_OP7_RLS UMETA(DisplayName = "Op7_Rls"),
	EN_OP8_PRS UMETA(DisplayName = "Op8_Prs"),
	EN_OP8_RLS UMETA(DisplayName = "Op8_Rls"),
	EN_OP9_PRS UMETA(DisplayName = "Op9_Prs"),
	EN_OP9_RLS UMETA(DisplayName = "Op9_Rls"),
};

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

	APlayerControllerBase();

	class ABattlefieldCharacterBase* CtrlPlayer;
	
	TMap<EnumControllerAxisInput, EnumCharacterAxisAction> mAxisInput;
	TMap<EnumControllerOperatorInput, EnumCharacterOnceAction> mOpInput;

public:
	UFUNCTION(Category = "Input", BlueprintCallable)
	ABattlefieldCharacterBase* GetControlledPlayer();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void Axis0_X(float v);
	void Axis0_Y(float v);
	void Axis1_X(float v);
	void Axis1_Y(float v);

	void Op0_Prs();
	void Op0_Rls();
	void Op1_Prs();
	void Op1_Rls();
	void Op2_Prs();
	void Op2_Rls();
	void Op3_Prs();
	void Op3_Rls();
	void Op4_Prs();
	void Op4_Rls();
	void Op5_Prs();
	void Op5_Rls();
	void Op6_Prs();
	void Op6_Rls();
	void Op7_Prs();
	void Op7_Rls();
	void Op8_Prs();
	void Op8_Rls();
	void Op9_Prs();
	void Op9_Rls();
};
