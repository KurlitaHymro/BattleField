// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputModComponent.h"
#include "GameplayAbilitySpec.h"
#include "AbilityInputBindingComponent.generated.h"

USTRUCT()
struct FAbilityInputBinding
{
	GENERATED_BODY()

	uint32 OnPressedHandle = 0;
	uint32 OnReleasedHandle = 0;
	TArray<int32> AbilitesStack;
};

/**
 * ����ǿ�������ת����InputID�Դ˼���/ȥ����ܡ�
 * һ���������Զ�Ӧ���������������ֻ����ջ���ļ��ܡ�
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLEFRAMERUNTIME_API UAbilityInputBindingComponent : public UPawnInputModComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SetupBinding(UInputAction* InputAction, int32 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TeardownAbilityBinding(int32 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TeardownActionBinding(UInputAction* InputAction);

protected:
	/** Native/BP Event to set up player controls */
	virtual void SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent) override;

	/** Native/BP Event to undo control setup */
	virtual void TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent) override;

private:
	void EnableBindings();
	void DisableBindings();
	void OnAbilityInputPressed(UInputAction* InputAction);
	void OnAbilityInputReleased(UInputAction* InputAction);

private:
	UPROPERTY(transient)
	class UBattleAbilitySystemComponent* BattleAbilityComponent;

	UPROPERTY(transient)
	TMap<UInputAction*, FAbilityInputBinding> MappedAbilities;
};
