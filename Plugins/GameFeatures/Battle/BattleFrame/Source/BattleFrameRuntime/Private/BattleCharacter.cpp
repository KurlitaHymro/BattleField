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

float ABattleCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    return ActualDamage;
}

float ABattleCharacter::CauseDamage_Implementation(float Damage, FDamageEvent const& DamageEvent, AController* EventVictim, AActor* DamageCauser)
{
    float ActualDamage = Damage;

    return ActualDamage;
}

UAbilitySystemComponent* ABattleCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent; 
}

UEquipmentSystemComponent* ABattleCharacter::GetEquipmentSystemComponent() const
{
    return EquipmentSystemComponent;
}
