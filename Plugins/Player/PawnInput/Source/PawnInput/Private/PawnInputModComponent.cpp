// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnInputModComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/InputDelegateBinding.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void UPawnInputModComponent::OnRegister()
{
	Super::OnRegister();

	UWorld* World = GetWorld();
	APawn* MyOwner = GetPawn<APawn>();

	if (ensure(MyOwner) && World->IsGameWorld())
	{
		MyOwner->ReceiveRestartedDelegate.AddDynamic(this, &UPawnInputModComponent::OnPawnRestarted);
		MyOwner->ReceiveControllerChangedDelegate.AddDynamic(this, &UPawnInputModComponent::OnControllerChanged);

		// If our pawn has an input component we were added after restart
		if (MyOwner->InputComponent)
		{
			OnPawnRestarted(MyOwner);
		}
	}
}

void UPawnInputModComponent::OnUnregister()
{
	UWorld* World = GetWorld();
	if (World && World->IsGameWorld())
	{
		ReleaseInputComponent();

		APawn* MyOwner = GetPawn<APawn>();
		if (MyOwner)
		{
			MyOwner->ReceiveRestartedDelegate.RemoveAll(this);
			MyOwner->ReceiveControllerChangedDelegate.RemoveAll(this);
		}
	}

	Super::OnUnregister();
}

void UPawnInputModComponent::OnPawnRestarted(APawn* Pawn)
{
	if (ensure(Pawn && Pawn == GetOwner()) && Pawn->InputComponent)
	{
		ReleaseInputComponent();

		if (Pawn->InputComponent)
		{
			SetupInputComponent(Pawn);
		}
	}
}

void UPawnInputModComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	// Only handle releasing, restart is a better time to handle binding
	if (ensure(Pawn && Pawn == GetOwner()) && OldController)
	{
		ReleaseInputComponent(OldController);
	}
}

void UPawnInputModComponent::SetupInputComponent(APawn* Pawn)
{
	InputComponent = CastChecked<UEnhancedInputComponent>(Pawn->InputComponent);

	if (ensureMsgf(InputComponent, TEXT("Project must use EnhancedInputComponent to support PlayerControlsComponent")))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem();
		check(Subsystem);

		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, InputPriority);
		}

		SetupPlayerControls(InputComponent);
	}
}

void UPawnInputModComponent::ReleaseInputComponent(AController* OldController)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem(OldController);
	if (Subsystem && InputComponent)
	{
		TeardownPlayerControls(InputComponent);

		if (InputMappingContext)
		{
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
	InputComponent = nullptr;
}

void UPawnInputModComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	// 派生类继承重写该方法，并调用BindInputAction()接口，绑定输入操作与行为。
}

void UPawnInputModComponent::TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{

}

UEnhancedInputLocalPlayerSubsystem* UPawnInputModComponent::GetEnhancedInputSubsystem(AController* OldController) const
{
	const APlayerController* PC = GetController<APlayerController>();
	if (!PC)
	{
		PC = Cast<APlayerController>(OldController);
		if (!PC)
		{
			return nullptr;
		}
	}

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	if (!LP)
	{
		return nullptr;
	}

	return LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}