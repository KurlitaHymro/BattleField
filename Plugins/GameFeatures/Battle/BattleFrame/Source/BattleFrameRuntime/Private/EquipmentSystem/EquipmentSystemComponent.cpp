// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/EquipmentSystemComponent.h"
#include "EquipmentSystem/EquipBase.h"

void UEquipmentSystemComponent::OnRegister()
{
	Super::OnRegister();

	UWorld* World = GetWorld();
	APawn* MyOwner = GetPawn<APawn>();

	if (ensure(MyOwner) && World->IsGameWorld())
	{
		MyOwner->ReceiveRestartedDelegate.AddDynamic(this, &UEquipmentSystemComponent::OnPawnRestarted);
		MyOwner->ReceiveControllerChangedDelegate.AddDynamic(this, &UEquipmentSystemComponent::OnControllerChanged);

		// If our pawn has an input component we were added after restart
		if (MyOwner->InputComponent)
		{
			OnPawnRestarted(MyOwner);
		}
	}
}

void UEquipmentSystemComponent::OnUnregister()
{
	UWorld* World = GetWorld();
	if (World && World->IsGameWorld())
	{
		//ReleaseInputComponent();

		APawn* MyOwner = GetPawn<APawn>();
		if (MyOwner)
		{
			MyOwner->ReceiveRestartedDelegate.RemoveAll(this);
			MyOwner->ReceiveControllerChangedDelegate.RemoveAll(this);
		}
	}

	Super::OnUnregister();
}

void UEquipmentSystemComponent::OnPawnRestarted(APawn* Pawn)
{
	if (ensure(Pawn && Pawn == GetOwner()))
	{
		
	}
}

void UEquipmentSystemComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	if (ensure(Pawn && Pawn == GetOwner()) && OldController)
	{
		
	}
}

void UEquipmentSystemComponent::PutOnEquip(AEquipBase* Equip, FName EquipSlot)
{
	if (ensure(Equip))
	{
		Equipped.Add(EquipSlot, Equip);

		Equip->OnPutOnDelegate.Broadcast(GetPawn<APawn>(), EquipSlot);
	}
}

void UEquipmentSystemComponent::TakeOffEquip(AEquipBase* Equip)
{
	auto Iterator = Equipped.CreateIterator();
	while (Iterator)
	{
		if (Iterator.Value() == Equip)
		{
			break;
		}
		++Iterator;
	}

	if (Iterator)
	{
		TakeOffEquipFromSlot(Iterator.Key());
		Iterator.Value()->OnTakeOffDelegate.Broadcast(GetPawn<APawn>());
		Iterator.RemoveCurrent();
	}
}

void UEquipmentSystemComponent::TakeOffEquipFromSlot(FName EquipSlot)
{
	auto Equip = FindEquipFromSlot(EquipSlot);
	if (Equip)
	{
		Equip->OnTakeOffDelegate.Broadcast(GetPawn<APawn>());
		Equipped.Remove(EquipSlot);
	}
}

void UEquipmentSystemComponent::TakeOffEquipAll()
{
	for (auto Equip : Equipped)
	{
		Equip.Value->OnTakeOffDelegate.Broadcast(GetPawn<APawn>());
	}
	Equipped.Empty();
}

AEquipBase* UEquipmentSystemComponent::FindEquipFromSlot(FName EquipSlot)
{
	auto Equip = Equipped.Find(EquipSlot);
	if (Equip)
	{
		return *Equip;
	}
	return nullptr;
}
