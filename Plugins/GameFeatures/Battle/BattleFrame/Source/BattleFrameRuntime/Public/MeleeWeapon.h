// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

USTRUCT()
struct FWeaponAnimHit
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<FName> HitPoint;
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

};
