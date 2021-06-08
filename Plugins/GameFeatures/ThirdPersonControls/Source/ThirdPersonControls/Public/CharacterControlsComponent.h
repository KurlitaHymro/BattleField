// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "Components/PawnComponent.h"
#include "CharacterControlsComponent.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONCONTROLS_API UCharacterControlsComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	/** Input mapping to add to the input system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Controls")
	class UInputMappingContext* InputMappingContext = nullptr;

	/** Priority to bind mapping context with */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Controls")
	int InputPriority = 0;

	// UActorComponent interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;

protected:
	/** Native/BP Event to set up player controls */
	UFUNCTION(BlueprintNativeEvent, Category = "Player Controls")
	void SetupPlayerControls(class UEnhancedInputComponent* PlayerInputComponent);

	/** Native/BP Event to undo control setup */
	UFUNCTION(BlueprintNativeEvent, Category = "Player Controls")
	void TeardownPlayerControls(class UEnhancedInputComponent* PlayerInputComponent);

	/** Wrapper function for binding to this input component */
	template<class UserClass, typename FuncType>
	bool BindInputAction(const class UInputAction* Action, const ETriggerEvent EventType, UserClass* Object, FuncType Func)
	{
		if (ensure(InputComponent != nullptr) && ensure(Action != nullptr))
		{
			InputComponent->BindAction(Action, EventType, Object, Func);
			return true;
		}

		return false;
	}

	/** Called when pawn restarts, bound to dynamic delegate */
	UFUNCTION()
	virtual void OnPawnRestarted(APawn* Pawn);

	/** Called when pawn restarts, bound to dynamic delegate */
	UFUNCTION()
	virtual void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);

	virtual void SetupInputComponent(APawn* Pawn);
	virtual void ReleaseInputComponent(AController* OldController = nullptr);
	class UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(AController* OldController = nullptr) const;

	/** The bound input component. */
	UPROPERTY(transient)
	class UEnhancedInputComponent* InputComponent;
};
