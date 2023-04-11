// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInputBindingComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "BattleAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UAbilityInputBindingComponent::SetupBinding(UInputAction* InputAction, int32 AbilityID)
{
	if (!BattleAbilityComponent)
	{
		EnableBindings();
	}

	FAbilityInputBinding* AbilityInputBinding = MappedAbilities.Find(InputAction);
	if (AbilityInputBinding)
	{
		BattleAbilityComponent->DisableAbility(AbilityInputBinding->AbilitesStack.Top());
	}
	else
	{
		AbilityInputBinding = &MappedAbilities.Add(InputAction);
	}

	AbilityInputBinding->AbilitesStack.Push(AbilityID);

	// Bind to InputAction
	if (InputComponent)
	{
		// Pressed event
		if (AbilityInputBinding->OnPressedHandle == 0)
		{
			AbilityInputBinding->OnPressedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UAbilityInputBindingComponent::OnAbilityInputPressed, InputAction).GetHandle();
		}

		// Released event
		if (AbilityInputBinding->OnReleasedHandle == 0)
		{
			AbilityInputBinding->OnReleasedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UAbilityInputBindingComponent::OnAbilityInputReleased, InputAction).GetHandle();
		}
	}
}

void UAbilityInputBindingComponent::TeardownAbilityBinding(int32 AbilityID)
{
	ensure(BattleAbilityComponent);

	// Find the mapping for this ability
	auto MappedIterator = MappedAbilities.CreateIterator();
	while (MappedIterator)
	{
		if (MappedIterator.Value().AbilitesStack.Top() == AbilityID)
		{
			break;
		}

		++MappedIterator;
	}

	if (MappedIterator)
	{
		FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();

		if (AbilityInputBinding.AbilitesStack.Remove(AbilityID) > 0)
		{
			if (AbilityInputBinding.AbilitesStack.Num() > 0)
			{
				BattleAbilityComponent->EnableAbility(AbilityID);
			}
			else
			{
				TeardownActionBinding(MappedIterator.Key());
			}
		}
	}
}

void UAbilityInputBindingComponent::TeardownActionBinding(UInputAction* InputAction)
{
	ensure(BattleAbilityComponent);

	if (FAbilityInputBinding* Binding = MappedAbilities.Find(InputAction))
	{
		if (InputComponent)
		{
			InputComponent->RemoveBindingByHandle(Binding->OnPressedHandle);
			InputComponent->RemoveBindingByHandle(Binding->OnReleasedHandle);
		}

		for (auto AbilityID : Binding->AbilitesStack)
		{
			BattleAbilityComponent->DisableAbility(AbilityID);
		}

		MappedAbilities.Remove(InputAction);
	}
}

void UAbilityInputBindingComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	DisableBindings();

	for (auto& Ability : MappedAbilities)
	{
		UInputAction* InputAction = Ability.Key;

		// Pressed event
		InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UAbilityInputBindingComponent::OnAbilityInputPressed, InputAction);

		// Released event
		InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UAbilityInputBindingComponent::OnAbilityInputReleased, InputAction);
	}

	EnableBindings();
}

void UAbilityInputBindingComponent::TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	DisableBindings();
}

void UAbilityInputBindingComponent::EnableBindings()
{
	AActor* MyOwner = GetOwner();
	check(MyOwner);

	BattleAbilityComponent = Cast<UBattleAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MyOwner));
	if (BattleAbilityComponent)
	{
		for (auto& InputBinding : MappedAbilities)
		{
			for (auto AbilityID : InputBinding.Value.AbilitesStack)
			{
				BattleAbilityComponent->EnableAbility(AbilityID);
			}
		}
	}
}

void UAbilityInputBindingComponent::DisableBindings()
{
	for (auto& InputBinding : MappedAbilities)
	{
		if (InputComponent)
		{
			InputComponent->RemoveBindingByHandle(InputBinding.Value.OnPressedHandle);
			InputComponent->RemoveBindingByHandle(InputBinding.Value.OnReleasedHandle);
		}

		if (BattleAbilityComponent)
		{
			for (auto AbilityID : InputBinding.Value.AbilitesStack)
			{
				BattleAbilityComponent->DisableAbility(AbilityID);
			}
		}
	}

	BattleAbilityComponent = nullptr;
}

void UAbilityInputBindingComponent::OnAbilityInputPressed(UInputAction* InputAction)
{
	if (BattleAbilityComponent)
	{
		FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding)
		{
			BattleAbilityComponent->AbilityLocalInputPressed(FoundBinding->AbilitesStack.Top());
		}
	}
}

void UAbilityInputBindingComponent::OnAbilityInputReleased(UInputAction* InputAction)
{
	if (BattleAbilityComponent)
	{
		FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding)
		{
			BattleAbilityComponent->AbilityLocalInputReleased(FoundBinding->AbilitesStack.Top());
		}
	}
}