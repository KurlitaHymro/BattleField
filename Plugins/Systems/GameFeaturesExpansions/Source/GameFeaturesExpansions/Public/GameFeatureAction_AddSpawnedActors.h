// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddSpawnedActors.generated.h"

/** Record for the an actor to spawn along with a game feature data. */
USTRUCT()
struct FSpawningActorEntry
{
	GENERATED_BODY()

	// What kind of actor to spawn
	UPROPERTY(EditAnywhere, Category = "Actor")
	TSubclassOf<AActor> ActorType;

	// Where to spawn the actor
	UPROPERTY(EditAnywhere, Category = "Actor|Transform")
	FTransform SpawnTransform;
};

/** Record for the game feature data. Specifies which actors to spawn for target worlds. */
USTRUCT()
struct FSpawningWorldActorsEntry
{
	GENERATED_BODY()

	// The world to spawn the actors for (can be left blank, in which case we'll spawn them for all worlds)
	UPROPERTY(EditAnywhere, Category = "Feature Data")
	TSoftObjectPtr<UWorld> TargetWorld;

	// The actors to spawn
	UPROPERTY(EditAnywhere, Category = "Feature Data")
	TArray<FSpawningActorEntry> Actors;

};

/**
 * 
 */
UCLASS(meta = (DisplayName = "Add Spawned Actors"))
class GAMEFEATURESEXPANSIONS_API UGameFeatureAction_AddSpawnedActors final : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif
	//~ End UGameFeatureAction interface

	//~ Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//~ End UObject interface

	/** List of actors */
	UPROPERTY(EditAnywhere, Category = "Actor")
	TArray<FSpawningWorldActorsEntry> ActorsList;

private:
	void HandleGameInstanceStart(UGameInstance* GameInstance);

	void AddToWorld(const FWorldContext& WorldContext);

	void Reset();
private:
	FDelegateHandle GameInstanceStartHandle;

	TArray<TWeakObjectPtr<AActor>> SpawnedActors;
};
