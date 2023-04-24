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

	UFUNCTION(Category = "Damage", BlueprintNativeEvent)
	float CauseDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventVictim, AActor* DamageCauser);
	virtual float CauseDamage_Implementation(float Damage, struct FDamageEvent const& DamageEvent, AController* EventVictim, AActor* DamageCauser);

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(Category = "Equipment", BlueprintCallable)
	virtual class UEquipmentSystemComponent* GetEquipmentSystemComponent() const;

protected:
	class UAbilitySystemComponent* AbilitySystemComponent;

	class UEquipmentSystemComponent* EquipmentSystemComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AActor* Weapon;
};
