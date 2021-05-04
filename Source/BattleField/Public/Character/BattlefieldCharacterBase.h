// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/ActorState.h"
#include "Common/CommonInterface.h"
#include "BattlefieldCharacterBase.generated.h"

UENUM(BlueprintType)
enum class EnumCharacterAxisAction : uint8
{
	EN_MOVE_Y UMETA(DisplayName = "MOVE_Y"),
	EN_MOVE_X UMETA(DisplayName = "MOVE_X"),
	EN_LOOK_Y UMETA(DisplayName = "LOOK_Y"),
	EN_LOOK_X UMETA(DisplayName = "LOOK_X"),
};

UENUM(BlueprintType)
enum class EnumCharacterOnceAction : uint8
{
	EN_ATTACK     UMETA(DisplayName = "ATTACK"),
	EN_JUMP       UMETA(DisplayName = "JUMP"),
	EN_JUMP_STOP  UMETA(DisplayName = "JUMP_STOP"),
	EN_SPEED_PLUS UMETA(DisplayName = "SPEED_PLUS"),
	EN_SPEED_SUB  UMETA(DisplayName = "SPEED_SUB"),
};

UCLASS()
class BATTLEFIELD_API ABattlefieldCharacterBase : public ACharacter
{
	GENERATED_BODY()

	TSubclassOf<UDamageType> DamageTypeClass;

	class UActorState* State;

	TArray<UStaticMeshComponent*> Weapon;

	class UWidgetComponent* WidgetComp;

public:
	// Sets default values for this character's properties
	ABattlefieldCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(Category = "Affiliation", EditAnywhere, BlueprintReadWrite)
	uint32 bIsEnemy : 1;

	UPROPERTY(Category = "State", EditAnywhere, BlueprintReadWrite)
	uint32 bIsValid : 1;

	UPROPERTY(Category = "State", EditAnywhere, BlueprintReadWrite)
	uint32 bIsInMotion : 1;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(Category = "State", EditAnywhere, BlueprintReadWrite)
	int RunSpeedChangeValue;

	UFUNCTION(Category = "Weapon", BlueprintCallable)
	void HitActor(AActor* targetActor);

	UFUNCTION(Category = "Weapon", BlueprintCallable)
	UStaticMeshComponent* GetWeaponMesh(int weaponId);

	UFUNCTION(Category = "State", BlueprintCallable)
	class UActorState* GetState();

	UFUNCTION(Category = "State", BlueprintNativeEvent)
	void CharacterStateUpdate(EnumActorStateItem state);
	virtual void CharacterStateUpdate_Implementation(EnumActorStateItem state);

	UFUNCTION(Category = "State", BlueprintNativeEvent)
	void CreateStateWidget();
	virtual void CreateStateWidget_Implementation();

private:
	void ComponentInit();

	void BeginPlayLoad();

	float CalDamage();

public:
	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void AxisInput(EnumCharacterAxisAction axis, float value = 0.f);

	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void OnceInput(EnumCharacterOnceAction action, float value = 0.f);

	/** Called for forwards/backward input */
	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void MoveForward(float Value);

	/** Called for side to side input */
	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void JumpStarted();

	/** Handler for when a touch input stops. */
	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void JumpStopped();

	/** Change Speed. */
	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void ChangeSpeed(int spd);

	/** Change Speed. */
	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void ResetSpeed();

	UFUNCTION(Category = "CharacterInput", BlueprintCallable)
	void MainNormalAttack();
};