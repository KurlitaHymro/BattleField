// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputModComponent.h"
#include "GameplayAbilitySpec.h"
#include "EnhancedInputComponent.h"
#include "BattleAbilitiesInputComponent.generated.h"

class UInputAction;

USTRUCT()
struct FAbilityInputBinding
{
	GENERATED_BODY()

	int32  InputID = 0;
	uint32 OnPressedHandle = 0;
	uint32 OnReleasedHandle = 0;
	TArray<FGameplayAbilitySpecHandle> BoundAbilitiesStack;
};

/**
 * 将增强输入操作转化成InputID以此激活/去激活技能
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLEABILITYATTRIBUTE_API UBattleAbilitiesInputComponent : public UPawnInputModComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(transient)
	class UAbilitySystemComponent* AbilityComponent;

	UPROPERTY(transient)
	TMap<UInputAction*, FAbilityInputBinding> MappedAbilities;

protected:
	/** Native/BP Event to set up player controls */
	void SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent);

	/** Native/BP Event to undo control setup */
	void TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent);

private:
	void SetupBindings();

	void TeardownBindings();

	void OnAbilityInputPressed(UInputAction* InputAction);

	void OnAbilityInputReleased(UInputAction* InputAction);

public:
	/** 对外接口  建立操作-技能绑定 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SetupBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle);

	/** 对外接口  废弃操作中所有绑定的技能 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TeardownActionBinding(UInputAction* InputAction);

	/** 对外接口  废弃技能与操作的绑定关系 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TeardownAbilityBinding(FGameplayAbilitySpecHandle AbilityHandle);

};
