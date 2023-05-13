// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayComponent/ActorSpawnerComponent.h"
#include "NavigationSystemAdapter.h"

// Sets default values for this component's properties
UActorSpawnerComponent::UActorSpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UActorSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	SpawnFromSet();
}

void UActorSpawnerComponent::AddToSet(TSubclassOf<AActor> ActorClass, int32 ActorNumber)
{
	int32* Number = ActorToSpawnSet.Find(ActorClass);
	if (Number)
	{
		*Number += ActorNumber;
	}
	else
	{
		ActorToSpawnSet.Emplace(ActorClass, ActorNumber);
	}
}

void UActorSpawnerComponent::SpawnFromSet()
{
	if (!ActorToSpawnSet.IsEmpty())
	{
		auto Iter = ActorToSpawnSet.CreateIterator();
		if (!bSpawnFromSetByOrder)
		{
			for (int32 offset = 0; offset < FMath::RandHelper(ActorToSpawnSet.Num()); offset++)
			{
				++Iter;
			}
		}
		if (Iter.Value() > 0)
		{
			Iter.Value()--;
			auto SpawnLocation = UNavigationSystemAdapter::GetInstance()->GetRandomLocationWithNavigationSystem(GetWorld(), GetComponentLocation(), RandomRadius);
			auto Actor = DoSpawn(Iter.Key(), SpawnLocation, GetComponentRotation());
			if (Iter.Value() == 0)
			{
				Iter.RemoveCurrent();
			}
		}
	}
}

AActor* UActorSpawnerComponent::DoSpawn(TSubclassOf<AActor> ActorClass, const FVector SpawnLocation, const FRotator SpawnRotation)
{
	FActorSpawnParameters SpawnConfig;
	SpawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	return GetWorld()->SpawnActor(ActorClass, &SpawnLocation, &SpawnRotation, SpawnConfig);
}