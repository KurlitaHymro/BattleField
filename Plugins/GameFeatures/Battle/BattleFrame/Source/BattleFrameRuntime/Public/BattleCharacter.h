// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CRCharacter.h"
#include "AbilitySystemInterface.h"
#include "BattleCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDieDelegate, ABattleCharacter*, SelfCharacter);

/**
 * 
 */
UCLASS()
class BATTLEFRAMERUNTIME_API ABattleCharacter : public ACRCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	ABattleCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called PreInitializeComponents
	virtual void PreInitializeComponents() override;

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(Category = "Equipment", BlueprintCallable)
	virtual class UEquipmentSystemComponent* GetEquipmentSystemComponent() const;

protected:
	class UAbilitySystemComponent* AbilitySystemComponent;

	class UEquipmentSystemComponent* EquipmentSystemComponent;

public:
	UPROPERTY(BlueprintAssignable)
	FCharacterDieDelegate OnBattleCharacterDie;

	UFUNCTION(BlueprintNativeEvent)
	void Die();
	virtual void Die_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Destroy();
	virtual void Destroy_Implementation();
};
