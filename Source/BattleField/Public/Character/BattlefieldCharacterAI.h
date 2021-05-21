// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BattlefieldCharacterBase.h"
#include "BattlefieldCharacterAI.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API ABattlefieldCharacterAI : public ABattlefieldCharacterBase
{
	GENERATED_BODY()

	// Sets default values for this character's properties
	ABattlefieldCharacterAI();

protected:
	virtual FRotator GetExceptRotatorInMotion_Implementation() override;

public:
	UPROPERTY(Category = Behavior, EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;

};
