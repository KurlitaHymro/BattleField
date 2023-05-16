// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacter.h"
#include "BattleAbilitySystemComponent.h"
#include "EquipmentSystem/EquipmentSystemComponent.h"

// Sets default values
ABattleCharacter::ABattleCharacter()
{
    AbilitySystemComponent = CreateDefaultSubobject<UBattleAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);

    EquipmentSystemComponent = CreateDefaultSubobject<UEquipmentSystemComponent>(TEXT("EquipmentSystemComponent"));
}

void ABattleCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ABattleCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void ABattleCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABattleCharacter::PreInitializeComponents()
{
    Super::PreInitializeComponents();
}

UAbilitySystemComponent* ABattleCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent; 
}

UEquipmentSystemComponent* ABattleCharacter::GetEquipmentSystemComponent() const
{
    return EquipmentSystemComponent;
}

void ABattleCharacter::Die_Implementation()
{
    OnBattleCharacterDie.Broadcast(this);

    EquipmentSystemComponent->TakeOffEquipAll();

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    FTimerHandle Handle;
    auto TimerDelegate = FTimerDelegate::CreateUObject(this, &ABattleCharacter::Destroy);
    GetWorldTimerManager().SetTimer(Handle, TimerDelegate, 5.0f, false);

    DetachFromControllerPendingDestroy();
}

void ABattleCharacter::Destroy_Implementation()
{
    AActor::Destroy();
}
