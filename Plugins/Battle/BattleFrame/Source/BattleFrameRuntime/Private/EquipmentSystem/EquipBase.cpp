// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/EquipBase.h"
#include "BattleCharacter.h"

// Sets default values
AEquipBase::AEquipBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EquipmentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = EquipmentRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(EquipmentRoot);
}

void AEquipBase::BeginPlay()
{
	Super::BeginPlay();

	OnPutOnDelegate.AddDynamic(this, &AEquipBase::OnPutOn);

	OnTakeOffDelegate.AddDynamic(this, &AEquipBase::OnTakeOff);
}

void AEquipBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnPutOnDelegate.RemoveAll(this);

	OnTakeOffDelegate.RemoveAll(this);
}

void AEquipBase::OnPutOn_Implementation(APawn* OwnerPawn, FName EquipSlot)
{
	OwnerCharacter = Cast<ABattleCharacter>(OwnerPawn);
	if (ensure(OwnerCharacter))
	{
		this->AttachToComponent(OwnerCharacter->GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
			EquipSlot);
		MeshComponent->SetSimulatePhysics(false);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEquipBase::OnTakeOff_Implementation(APawn* OwnerPawn)
{
	if (OwnerCharacter && OwnerCharacter == OwnerPawn)
	{
		this->DetachRootComponentFromParent(true);
		OwnerCharacter = nullptr;
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
}

UMeshComponent* AEquipBase::GetMesh()
{
	return MeshComponent;
}

