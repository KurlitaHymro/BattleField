// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPutOnEquipDelegate, APawn*, OwnerPawn, FName, EquipSlot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTakeOffEquipDelegate, APawn*, OwnerPawn);

UCLASS(abstract)
class BATTLEFRAMERUNTIME_API AEquipBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipBase();

	UPROPERTY(BlueprintAssignable)
	FPutOnEquipDelegate OnPutOnDelegate;

	UPROPERTY(BlueprintAssignable)
	FTakeOffEquipDelegate OnTakeOffDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnPutOn(APawn* OwnerPawn, FName EquipSlot);
	virtual void OnPutOn_Implementation(APawn* OwnerPawn, FName EquipSlot);

	UFUNCTION(BlueprintNativeEvent)
	void OnTakeOff(APawn* OwnerPawn);
	virtual void OnTakeOff_Implementation(APawn* OwnerPawn);

protected:
	class ACharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> EquipmentRoot;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMeshComponent> MeshComponent;

public:
	UFUNCTION()
	UMeshComponent* GetMesh();
};
