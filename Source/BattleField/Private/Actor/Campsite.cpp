// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Campsite.h"
#include "Common/CommonInterface.h"
#include "Component/ActorSpawner.h"
#include "Character/BattlefieldCharacterAI.h"

// Sets default values
ACampsite::ACampsite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	curNum = 0;
	Spawner = CreateDefaultSubobject<UActorSpawner>(TEXT("Spawner"));

}

// Called when the game starts or when spawned
void ACampsite::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACampsite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACampsite::SetSpawnRadius(float r)
{
	SpawnRadius = r;
}

void ACampsite::AddToCampsite(int id, int num)
{
	UE_LOG(RunLog, Warning, TEXT("Push To Campsite"));
	unsigned int& characterNum = camp.FindOrAdd(id);
	if (static_cast<int>(characterNum) + num > 0) {
		characterNum += num;
		curNum += num;
	} else {
		curNum -= characterNum;
		camp.Remove(id);
	}
}

void ACampsite::CampsiteRandomSpawn(int num)
{
	int rsv = num;
	while (rsv > 0) {
		int eleNum = camp.Num();
		if (!eleNum || !curNum) {
			return;
		}
		unsigned int seed = rand() % curNum;
		for (auto& item : camp) {
			if (seed < item.Value) {
				item.Value--;
				CampsiteSpawnByInfo(item.Key);
				if (item.Value <= 0) {
					camp.Remove(item.Key);
				}
				curNum--;
			} else {
				seed -= item.Value;
			}
		}

		auto it = camp.CreateIterator();

		it.Key();
		rsv--;
	}
}

void ACampsite::CampsiteSpawnByInfo(int id)
{
	UDataTable* dataTable = UCommonInterface::GetCharacterPropertyTable();
	if (!dataTable) {
		// dataTable是GameMode的静态成员。
		return;
	}
	FString rowName = FString::FromInt(id);
	FPropertyTableRow* item = dataTable->FindRow<FPropertyTableRow>(FName(rowName), TEXT("CharacterName"), true);

	if (item) {
		AActor* spawnActor = Spawner->SpawnInScope(ABattlefieldCharacterAI::StaticClass(), SpawnRadius);
		if (spawnActor) {
			ABattlefieldCharacterAI* AICharacter = Cast<ABattlefieldCharacterAI>(spawnActor);
			if (AICharacter) {
				AICharacter->bIsEnemy = bIsEnemy;
				AICharacter->GetState()->InitCharacterStateByInfo(*item);
			}
		} else {
			UE_LOG(RunLog, Warning, TEXT("SpawnActor<ABattlefieldCharacterAI> Fail"));
		}
	} else {
		UE_LOG(RunLog, Error, TEXT("Fail To Read Item"));
	}
}