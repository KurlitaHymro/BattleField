// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAbilitySystemComponent.h"

void UBattleAbilitySystemComponent::ClearAddedAbilities()
{
	for (const auto& Ability : AddedAbilityHandle)
	{
		SetRemoveAbilityOnEnd(Ability);
	}
	AddedAbilityHandle.Empty(DefaultAbilities.Num());
}

void UBattleAbilitySystemComponent::ClearAddedAttributes()
{
	for (const auto& Attribute : AddedAttributeSet)
	{
		GetSpawnedAttributes_Mutable().Remove(Attribute);
	}
	AddedAttributeSet.Empty(DefaultAttributes.Num());
}

void UBattleAbilitySystemComponent::GrantDefaultAbilities()
{
	AddedAbilityHandle.Reserve(DefaultAbilities.Num());
	for (const auto& Ability : DefaultAbilities)
	{
		if (Ability.AbilityType)
		{
			AddedAbilityHandle.Add(GiveAbility(FGameplayAbilitySpec(Ability.AbilityType)));
		}
	}
}

void UBattleAbilitySystemComponent::GrantDefaultAttributes()
{
	AddedAttributeSet.Reserve(DefaultAttributes.Num());
	for (const auto& Attribute : DefaultAttributes)
	{
		if (Attribute.AttributeSetType) {
			UAttributeSet* AttributeSet = NewObject<UAttributeSet>(this, Attribute.AttributeSetType);
			if (Attribute.InitializationData)
			{
				AttributeSet->InitFromMetaDataTable(Attribute.InitializationData);
			}
			AddedAttributeSet.Add(AttributeSet);
			AddAttributeSetSubobject(AttributeSet);
		}
	}
}

void UBattleAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (AbilityActorInfo)
	{
		if (UGameInstance* GameInstance = InOwnerActor->GetGameInstance())
		{
			// Sign up for possess/unpossess events so that we can update the cached AbilityActorInfo accordingly
			GameInstance->GetOnPawnControllerChanged().AddDynamic(this, &UBattleAbilitySystemComponent::OnPawnControllerChanged);
		}
	}

	ClearAddedAbilities();
	ClearAddedAttributes();
	GrantDefaultAbilities();
	GrantDefaultAttributes();
}

void UBattleAbilitySystemComponent::BeginDestroy()
{
	if (AbilityActorInfo && AbilityActorInfo->OwnerActor.IsValid())
	{
		if (UGameInstance* GameInstance = AbilityActorInfo->OwnerActor->GetGameInstance())
		{
			GameInstance->GetOnPawnControllerChanged().RemoveAll(this);
		}
	}

	Super::BeginDestroy();
}

void UBattleAbilitySystemComponent::OnPawnControllerChanged(APawn* Pawn, AController* NewController)
{
	if (AbilityActorInfo && AbilityActorInfo->OwnerActor == Pawn && AbilityActorInfo->PlayerController != NewController)
	{
		// Reinit the cached ability actor info (specifically the player controller)
		AbilityActorInfo->InitFromActor(AbilityActorInfo->OwnerActor.Get(), AbilityActorInfo->AvatarActor.Get(), this);
	}
}

