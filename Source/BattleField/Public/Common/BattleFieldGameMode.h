// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BattleFieldGameMode.generated.h"

/* 日志类型声明 */
DECLARE_LOG_CATEGORY_EXTERN(LoadLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(RunLog, Log, All);

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API ABattleFieldGameMode : public AGameMode
{
	GENERATED_BODY()

	ABattleFieldGameMode();

	~ABattleFieldGameMode();

public:
	UPROPERTY(Category = GameMode, EditAnywhere, BlueprintReadWrite)
	uint32 bFriendlyFire : 1;

	static class UBattleFieldDelegation* Delegation;

	static class UCharacterProperty* PropertyTableRow;

	UFUNCTION(Category = "Interface", BlueprintCallable)
	class UBattleFieldDelegation* GetBattleFieldDelegation();

	UFUNCTION(Category = "Interface", BlueprintCallable)
	class UCharacterProperty* GetCharacterProperty();
};
