// Fill out your copyright notice in the Description page of Project Settings.


#include "GrantAbilitySystemComponent.h"

void UGrantAbilitySystemComponent::ClearGrantedAbilities()
{
	for (const auto& Ability : GrantedAbilitiesHandle)
	{
		SetRemoveAbilityOnEnd(Ability.Value);
	}
	GrantedAbilitiesHandle.Empty();
}

void UGrantAbilitySystemComponent::ClearGrantedAttributes()
{
	for (const auto& Attribute : GrantedAttributesSet)
	{
		GetSpawnedAttributes_Mutable().Remove(Attribute);
	}
	GrantedAttributesSet.Empty(DefaultAttributes.Num());
}

void UGrantAbilitySystemComponent::GrantDefaultAbilities()
{
	for (const auto& Ability : DefaultAbilities)
	{
		if (Ability.AbilityType)
		{
			FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(FGameplayAbilitySpec(Ability.AbilityType));
			GrantedAbilitiesHandle.Add(Ability.AbilityType, AbilityHandle);
		}
	}
}

void UGrantAbilitySystemComponent::GrantDefaultAttributes()
{
	GrantedAttributesSet.Reserve(DefaultAttributes.Num());
	for (const auto& Attribute : DefaultAttributes)
	{
		if (Attribute.AttributeSetType) {
			UAttributeSet* AttributeSet = NewObject<UAttributeSet>(this, Attribute.AttributeSetType);
			if (Attribute.InitializationData)
			{
				AttributeSet->InitFromMetaDataTable(Attribute.InitializationData);
			}
			GrantedAttributesSet.Add(AttributeSet);
			AddAttributeSetSubobject(AttributeSet);
		}
	}
}

void UGrantAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (AbilityActorInfo)
	{
		if (UGameInstance* GameInstance = InOwnerActor->GetGameInstance())
		{
			// Sign up for possess/unpossess events so that we can update the cached AbilityActorInfo accordingly
			GameInstance->GetOnPawnControllerChanged().AddDynamic(this, &UGrantAbilitySystemComponent::OnPawnControllerChanged);
		}
	}

	ClearGrantedAbilities();
	
	ClearGrantedAttributes();
	
	GrantDefaultAbilities();
	
	GrantDefaultAttributes();
}

void UGrantAbilitySystemComponent::BeginDestroy()
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

void UGrantAbilitySystemComponent::OnPawnControllerChanged(APawn* Pawn, AController* NewController)
{
	if (AbilityActorInfo && AbilityActorInfo->OwnerActor == Pawn && AbilityActorInfo->PlayerController != NewController)
	{
		// Reinit the cached ability actor info (specifically the player controller)
		AbilityActorInfo->InitFromActor(AbilityActorInfo->OwnerActor.Get(), AbilityActorInfo->AvatarActor.Get(), this);
	}
}