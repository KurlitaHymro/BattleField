// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInputBindingComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

namespace AbilityInputBindingComponent_Impl
{
	constexpr int32 InvalidInputID = 0;
	int32 IncrementingInputID = InvalidInputID;

	static int32 GetNextInputID()
	{
		return ++IncrementingInputID;
	}
}

void UAbilityInputBindingComponent::SetupBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle)
{
	ensure(AbilityComponent);

	FAbilityInputBinding* AbilityInputBinding = MappedAbilities.Find(InputAction);
	if (AbilityInputBinding)
	{
		// �����Ѵ���ʱ�ȷ���ջ������
		FGameplayAbilitySpec* OldBoundAbility = AbilityComponent->FindAbilitySpecFromHandle(AbilityInputBinding->BoundAbilitiesStack.Top());
		if (OldBoundAbility && OldBoundAbility->InputID == AbilityInputBinding->InputID)
		{
			OldBoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
		}
	}
	else
	{
		// ����������ʱ���µ�ID����ò���
		AbilityInputBinding = &MappedAbilities.Add(InputAction);
		AbilityInputBinding->InputID = AbilityInputBindingComponent_Impl::GetNextInputID();
	}

	// �����󶨼����Ƶ�ջ����ͬ��ID
	AbilityInputBinding->BoundAbilitiesStack.Push(AbilityHandle);
	FGameplayAbilitySpec* BindingAbility = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
	if (BindingAbility)
	{
		BindingAbility->InputID = AbilityInputBinding->InputID;
	}

	// ���ӵ��������
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

void UAbilityInputBindingComponent::TeardownAbilityBinding(FGameplayAbilitySpecHandle AbilityHandle)
{
	ensure(AbilityComponent);

	if (FGameplayAbilitySpec* FoundAbility = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle))
	{
		// Find the mapping for this ability
		auto MappedIterator = MappedAbilities.CreateIterator();
		while (MappedIterator)
		{
			if (MappedIterator.Value().InputID == FoundAbility->InputID)
			{
				break;
			}

			++MappedIterator;
		}

		if (MappedIterator)
		{
			FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();

			if (AbilityInputBinding.BoundAbilitiesStack.Remove(AbilityHandle) > 0)
			{
				if (AbilityInputBinding.BoundAbilitiesStack.Num() > 0)
				{
					// �������������������ԭջ��InputID
					FGameplayAbilitySpec* StackedAbility = AbilityComponent->FindAbilitySpecFromHandle(AbilityInputBinding.BoundAbilitiesStack.Top());
					if (StackedAbility && StackedAbility->InputID == 0)
					{
						StackedAbility->InputID = AbilityInputBinding.InputID;
					}
				}
				else
				{
					// ��������������������Ҳ�Ƴ��˲���
					// NOTE: This will invalidate the `AbilityInputBinding` ref above
					TeardownActionBinding(MappedIterator.Key());
				}
				// DO NOT act on `AbilityInputBinding` after here (it could have been removed)

				FoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
			}
		}
	}
}

void UAbilityInputBindingComponent::TeardownActionBinding(UInputAction* InputAction)
{
	if (FAbilityInputBinding* Binding = MappedAbilities.Find(InputAction))
	{
		// �Ƴ���ǿ�����������
		if (InputComponent)
		{
			InputComponent->RemoveBindingByHandle(Binding->OnPressedHandle);
			InputComponent->RemoveBindingByHandle(Binding->OnReleasedHandle);
		}

		// ���������ID��ƥ��ļ�������ID
		for (FGameplayAbilitySpecHandle AbilityHandle : Binding->BoundAbilitiesStack)
		{
			FGameplayAbilitySpec* AbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
			if (AbilitySpec && AbilitySpec->InputID == Binding->InputID)
			{
				AbilitySpec->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
			}
		}

		MappedAbilities.Remove(InputAction);
	}
}

void UAbilityInputBindingComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	DisableBindings();

	/** ���������Pressed/Released�󶨵����в���IA��Started/Completed��
	 *  �ٸ��ݾ����IA����InputID�����ü���ϵͳ��Pressed/Released�ӿڴ�����������
	 */
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

	// �������ID
	AbilityComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MyOwner);
	if (AbilityComponent)
	{
		for (auto& InputBinding : MappedAbilities)
		{
			const int32 NewInputID = AbilityInputBindingComponent_Impl::GetNextInputID();
			InputBinding.Value.InputID = NewInputID;

			// ͬ������ID����������ID
			for (FGameplayAbilitySpecHandle AbilityHandle : InputBinding.Value.BoundAbilitiesStack)
			{
				FGameplayAbilitySpec* FoundAbility = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
				if (FoundAbility != nullptr)
				{
					FoundAbility->InputID = NewInputID;
				}
			}
		}
	}
}

void UAbilityInputBindingComponent::DisableBindings()
{
	for (auto& InputBinding : MappedAbilities)
	{
		// �Ƴ���ǿ�����������
		if (InputComponent)
		{
			InputComponent->RemoveBindingByHandle(InputBinding.Value.OnPressedHandle);
			InputComponent->RemoveBindingByHandle(InputBinding.Value.OnReleasedHandle);
		}

		// ���������ID��ƥ��ļ�������ID
		if (AbilityComponent)
		{
			const int32 ExpectedInputID = InputBinding.Value.InputID;

			for (FGameplayAbilitySpecHandle AbilityHandle : InputBinding.Value.BoundAbilitiesStack)
			{
				FGameplayAbilitySpec* FoundAbility = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
				if (FoundAbility && FoundAbility->InputID == ExpectedInputID)
				{
					FoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
				}
			}
		}
	}

	AbilityComponent = nullptr;
}

void UAbilityInputBindingComponent::OnAbilityInputPressed(UInputAction* InputAction)
{
	// The AbilitySystemComponent may not have been valid when we first bound input... try again.
	if (!AbilityComponent)
	{
		EnableBindings();
	}

	if (AbilityComponent)
	{
		FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding && ensure(FoundBinding->InputID != AbilityInputBindingComponent_Impl::InvalidInputID))
		{
			AbilityComponent->AbilityLocalInputPressed(FoundBinding->InputID);
		}
	}
}

void UAbilityInputBindingComponent::OnAbilityInputReleased(UInputAction* InputAction)
{
	if (AbilityComponent)
	{
		FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding && ensure(FoundBinding->InputID != AbilityInputBindingComponent_Impl::InvalidInputID))
		{
			AbilityComponent->AbilityLocalInputReleased(FoundBinding->InputID);
		}
	}
}