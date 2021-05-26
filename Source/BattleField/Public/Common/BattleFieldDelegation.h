// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Component/ActorState.h"
#include "BattleFieldDelegation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FuncCharacterStateChange, class ABattlefieldCharacterBase*, character, EnumActorStateItem, state);

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API UBattleFieldDelegation : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FuncCharacterStateChange PlayerStateChange;
};
