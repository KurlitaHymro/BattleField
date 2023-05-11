// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ActorSpawnerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BATTLEFIELD_API UActorSpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorSpawnerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<AActor>, int32> ActorToSpawnSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawnFromSetByOrder;

public:
	UFUNCTION(BlueprintCallable)
	void AddToSet(TSubclassOf<AActor> ActorClass, int32 ActorNumber);

	UFUNCTION(BlueprintCallable)
	void SpawnFromSet();

	UFUNCTION(BlueprintCallable)
	FVector GetRandomLocationWithNavigationSystem();

	UFUNCTION(BlueprintCallable)
	AActor* DoSpawn(TSubclassOf<AActor> ActorClass, const FVector SpawnLocation, const FRotator SpawnRotation);
};
