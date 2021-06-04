// Fill out your copyright notice in the Description page of Project Settings.


#include "CRGameMode.h"
#include "CRGameState.h"
#include "CRPlayerController.h"
#include "CRPlayerState.h"
#include "CRPawn.h"

ACRGameMode::ACRGameMode()
{
	GameStateClass = ACRGameState::StaticClass();
	PlayerControllerClass = ACRPlayerController::StaticClass();
	PlayerStateClass = ACRPlayerState::StaticClass();
	DefaultPawnClass = ACRPawn::StaticClass();
}
