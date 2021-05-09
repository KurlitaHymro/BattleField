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
	UE_LOG(RunLog, Warning, TEXT("Push To Campsite Id:%d Num:%d"), id, num);
	unsigned int& characterNum = camp.FindOrAdd(id);
	if (static_cast<int>(characterNum) + num > 0) {
		characterNum += num;
		curNum += num;
	} else {
		curNum -= characterNum;
		camp.Remove(id);
	}
}

int ACampsite::CampsiteRandomSpawn(int num)
{
	int rsv = num;
	int spawnCnt = 0;
	while (rsv > 0) {
		int eleNum = camp.Num();
		if (!eleNum || !curNum) {
			return 0;
		}
		unsigned int seed = rand() % curNum;
		for (auto& item : camp) {
			if (seed < item.Value) {
				bool spawnResult = CampsiteSpawnByInfo(item.Key);
				if (spawnResult) {
					spawnCnt++;
					item.Value--;
					if (item.Value <= 0) {
						camp.Remove(item.Key);
					}
					curNum--;
					rsv--;
					UE_LOG(RunLog, Warning, TEXT("Success To Spawn, rsv = %d"), curNum);
				}
				break;
			} else {
				seed -= item.Value;
			}
		}
		rsv--;
	}
	return num - spawnCnt;
}

void ACampsite::InitCharacterInfo(ABattlefieldCharacterAI* AI)
{
	AI->bIsEnemy = bIsEnemy;
	AI->CharacterStateUpdate(EnumActorStateItem::EN_HP);
}

bool ACampsite::CampsiteSpawnByInfo(int id)
{
	UDataTable* dataTable = UCommonInterface::GetCharacterPropertyTable();
	if (!dataTable) {
		return false;
	}
	FString rowName = FString::FromInt(id);
	FPropertyTableRow* item = dataTable->FindRow<FPropertyTableRow>(FName(rowName), TEXT("CharacterName"), true);

	if (item) {
		AActor* spawnActor = Spawner->SpawnInScope(ABattlefieldCharacterAI::StaticClass(), SpawnRadius);
		if (spawnActor) {
			ABattlefieldCharacterAI* AICharacter = Cast<ABattlefieldCharacterAI>(spawnActor);
			if (AICharacter) {
				AICharacter->GetState()->InitCharacterStateByInfo(*item);
				InitCharacterInfo(AICharacter);
				return true;
			} else {
				UE_LOG(RunLog, Error, TEXT("Spawn Type Error"));
				return false;
			}
		} else {
			UE_LOG(RunLog, Warning, TEXT("Spawn Fail"));
			return false;
		}
	} else {
		UE_LOG(RunLog, Error, TEXT("Fail To Read Item"));
		return false;
	}
}