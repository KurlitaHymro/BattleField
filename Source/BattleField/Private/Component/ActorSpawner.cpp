// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ActorSpawner.h"
#include "NavigationSystem.h"

// Sets default values for this component's properties
UActorSpawner::UActorSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* UActorSpawner::SpawnInScope(UClass* actorClass, float r)
{
	UWorld* const world = GetWorld();
	if (world) {
		/* 获取导航 */
		if (!navSys) {
			navSys = UNavigationSystemV1::GetCurrent(world);
		}
		/* 计算位置 */
		FNavLocation targetLocation;
		bool readyToSpawn =
			navSys->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), r, targetLocation);
		if (!readyToSpawn) {
			return nullptr;
		}
		FVector toSpawnLocation = targetLocation.Location;
		FRotator toSpawnRotation = GetOwner()->GetActorRotation();
		/* 生成 */
		AActor* spawnCharacter =
			GetWorld()->SpawnActor(actorClass, &toSpawnLocation, &toSpawnRotation);
		if (!spawnCharacter) {
			return nullptr;
		}
	}
	return nullptr;
}