// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputModComponent.h"
#include "GameplayAbilitySpec.h"
#include "EnhancedInputComponent.h"
#include "AbilitiesInputComponent.generated.h"

USTRUCT()
struct FAbilitiesStack
{
	GENERATED_BODY()

	int32  InputID = 0;
	uint32 OnPressedHandle = 0;
	uint32 OnReleasedHandle = 0;
	TArray<FGameplayAbilitySpecHandle> BoundAbilitiesStack;
};

class UInputAction;

/**
 * Pawn can use Abilities by InputAction.
 * ����ǿ�������ת����InputID�Դ˼���/ȥ�����
 * һ���������Զ�Ӧ���������������ֻ����ջ���ļ��ܡ�
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ABILITIESINPUTRUNTIME_API UAbilitiesInputComponent : public UPawnInputModComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(transient)
	class UAbilitySystemComponent* AbilityComponent;

	UPROPERTY(transient)
	TMap<UInputAction*, FAbilitiesStack> MappedAbilities;

	void SetupBindings();

	void TeardownBindings();

	void OnAbilityInputPressed(UInputAction* InputAction);

	void OnAbilityInputReleased(UInputAction* InputAction);

public:
	/** �������ܶԲ����İ󶨹�ϵ */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SetupBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle);

	/** �������������а󶨵ļ��� */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TeardownActionBinding(UInputAction* InputAction);

	/** ��������������İ󶨹�ϵ */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TeardownAbilityBinding(FGameplayAbilitySpecHandle AbilityHandle);

protected:
	/** Native/BP Event to set up player controls */
	void SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent);

	/** Native/BP Event to undo control setup */
	void TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent);

};
