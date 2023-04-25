// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "EquipmentSystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFRAMERUNTIME_API UEquipmentSystemComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	//~ Begin UActorComponent interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	//~ End UActorComponent interface

protected:
	/** Called when pawn restarts, bound to dynamic delegate */
	UFUNCTION()
	virtual void OnPawnRestarted(APawn* Pawn);

	/** Called when pawn restarts, bound to dynamic delegate */
	UFUNCTION()
	virtual void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);

public:
	UFUNCTION(BlueprintCallable)
	void PutOnEquip(class AEquipBase* Equip, FName EquipSlot);

	UFUNCTION(BlueprintCallable)
	void TakeOffEquip(class AEquipBase* Equip);

	UFUNCTION(BlueprintCallable)
	void TakeOffEquipFromSlot(FName EquipSlot);

	UFUNCTION(BlueprintCallable)
	void TakeOffEquipAll();

	UFUNCTION(BlueprintCallable)
	class AEquipBase* FindEquipFromSlot(FName EquipSlot);

private:
	TMap<FName, AEquipBase*> Equipped;
};
