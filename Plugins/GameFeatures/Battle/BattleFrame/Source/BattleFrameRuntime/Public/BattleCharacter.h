// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CRCharacter.h"
#include "AbilitySystemInterface.h"
#include "BattleCharacter.generated.h"

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
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// IAbilitySystemInterface - Returns our Ability System Component. 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	class UAbilitySystemComponent* AbilitySystemComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<class UStaticMeshComponent*> Weapons;
};
