// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonInterface.h"
#include "ActorSpawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEFIELD_API UActorSpawner : public UActorComponent
{
	GENERATED_BODY()

	class UNavigationSystemV1* navSys;
public:	
	// Sets default values for this component's properties
	UActorSpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Category = "Spawner")
	AActor* SpawnInScope(UClass* actorClass, float r = 0.f);
};
