// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Campsite.generated.h"

UCLASS()
class BATTLEFIELD_API ACampsite : public AActor
{
	GENERATED_BODY()
	
	TMap<int, unsigned int> camp; // 存储兵种Id和个数

	unsigned int curNum; // 营地现存个数

	class UActorSpawner* Spawner;

public:	
	// Sets default values for this actor's properties
	ACampsite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = "Affiliation", EditAnywhere, BlueprintReadWrite)
	uint32 bIsEnemy : 1;

	UPROPERTY(Category = "Campsite", EditAnywhere, BlueprintReadWrite)
	float SpawnRadius = 0.f;

	UFUNCTION(Category = "Campsite", BlueprintCallable)
	void SetSpawnRadius(float r);

	UFUNCTION(Category = "Campsite", BlueprintCallable)
	void AddToCampsite(int id, int num);

	UFUNCTION(Category = "Campsite", BlueprintCallable)
	void CampsiteRandomSpawn(int num);

	/* 根据ID查询属性信息，然后调用UCharacterSpawnMng组件的接口，生成一个士兵。 */
	UFUNCTION(Category = "Campsite", BlueprintCallable)
	void CampsiteSpawnByInfo(int id);
};
