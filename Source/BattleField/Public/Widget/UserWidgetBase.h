// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/CommonInterface.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFIELD_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HP;

	UFUNCTION(Category = Widget, BlueprintCallable)
	void StateUpdate(class ABattlefieldCharacterBase* character, EnumActorStateItem stateItem);

	UFUNCTION(Category = Widget, BlueprintCallable)
	void SetHpColor(FLinearColor InColor);
};
