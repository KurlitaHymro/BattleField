// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEFIELD_API UCharacterAction : public UActorComponent
{
	GENERATED_BODY()

	class ABattlefieldCharacterBase* owner;

	class UAnimInstance* AnimInstance;

	class UAnimMontage* DeadAnmi;

	class UAnimMontage* NormalAttackCombo;
	uint32 NormalAttackComboNum = 0;
	uint32 NormalAttackComboIdx;

	int32 saveInput;
public:	
	// Sets default values for this component's properties
	UCharacterAction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Category = "Action")
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(Category = "Action")
	void OnMontageStarted(UAnimMontage* Montage);

	UFUNCTION(Category = "Action")
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Category = "Action", BlueprintCallable)
	void Dead();

	UFUNCTION(Category = "Action", BlueprintCallable)
	void MainNormalAttack();
};
