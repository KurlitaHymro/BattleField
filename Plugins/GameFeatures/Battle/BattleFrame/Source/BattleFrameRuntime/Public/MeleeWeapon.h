// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

USTRUCT()
struct FWeaponHitSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName HitPoint;

	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (DisplayName = "Density | Land | Length"))
	FVector TraceHalfSize;
};

UCLASS()
class BATTLEFRAMERUNTIME_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UMeshComponent> WeaponMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<FWeaponHitSlot> HitPoints;
};
