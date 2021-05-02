// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Common/BattleFieldDelegation.h"
#include "Common/BattleFieldGameMode.h"
#include "Interface/CharacterProperty.h"
#include "CommonInterface.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API UCommonInterface : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = "Interface", BlueprintCallable)
	static class ABattleFieldGameMode* GetGameModeInst();

	UFUNCTION(Category = "Interface", BlueprintCallable)
	static UBattleFieldDelegation* GetDelegation();

	UFUNCTION(Category = "Interface", BlueprintCallable)
	static UDataTable* GetCharacterPropertyTable();

	static class ABattleFieldGameMode* GameModeInst;
};
