// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/Components/PawnControlsComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/InputDelegateBinding.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void UPawnControlsComponent::OnRegister()
{
	Super::OnRegister();

	UWorld* World = GetWorld();
	APawn* MyOwner = GetPawn<APawn>();

	if (ensure(MyOwner) && World->IsGameWorld())
	{
		MyOwner->ReceiveRestartedDelegate.AddDynamic(this, &UPawnControlsComponent::OnPawnRestarted);
		MyOwner->ReceiveControllerChangedDelegate.AddDynamic(this, &UPawnControlsComponent::OnControllerChanged);

		// If our pawn has an input component we were added after restart
		if (MyOwner->InputComponent)
		{
			OnPawnRestarted(MyOwner);
		}
	}
}

void UPawnControlsComponent::OnUnregister()
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

void UPawnControlsComponent::OnPawnRestarted(APawn* Pawn)
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

void UPawnControlsComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	// Only handle releasing, restart is a better time to handle binding
	if (ensure(Pawn && Pawn == GetOwner()) && OldController)
	{
		ReleaseInputComponent(OldController);
	}
}

void UPawnControlsComponent::SetupInputComponent(APawn* Pawn)
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

void UPawnControlsComponent::ReleaseInputComponent(AController* OldController)
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

void UPawnControlsComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	// 派生类继承重写该方法，并调用BindInputAction()接口，绑定输入操作与行为。
}

void UPawnControlsComponent::TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{

}

UEnhancedInputLocalPlayerSubsystem* UPawnControlsComponent::GetEnhancedInputSubsystem(AController* OldController) const
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