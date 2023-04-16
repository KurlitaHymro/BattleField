// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacter.h"
#include "BattleAbilitySystemComponent.h"

// Sets default values
ABattleCharacter::ABattleCharacter()
{
    AbilitySystemComponent = CreateDefaultSubobject<UBattleAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
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
    return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

UAbilitySystemComponent* ABattleCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}
