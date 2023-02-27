// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

namespace ActionAbilityInputID
{
	constexpr int32 InvalidInputID = 0;
	int32 IncrementingInputID = InvalidInputID;

	static int32 GetNewInputID()
	{
		return ++IncrementingInputID;
	}
}

void UAbilitiesInputComponent::SetupBindings()
{
	AActor* MyOwner = GetOwner();
	check(MyOwner);

	// �������ID
	AbilityComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MyOwner);
	if (AbilityComponent)
	{
		for (auto& Binding : MappedAbilities)
		{
			const auto NewInputID = ActionAbilityInputID::GetNewInputID();
			Binding.Value.InputID = NewInputID;

			// ͬ������ID����������ID
			for (auto AbilityHandle : Binding.Value.BoundAbilitiesStack)
			{
				auto AbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
				if (AbilitySpec)
				{
					AbilitySpec->InputID = NewInputID;
				}
			}
		}
	}
}

void UAbilitiesInputComponent::TeardownBindings()
{
	// �Ƴ���ǿ�����������
	if (InputComponent)
	{
		for (auto& Binding : MappedAbilities)
		{
			InputComponent->RemoveBindingByHandle(Binding.Value.OnPressedHandle);
			InputComponent->RemoveBindingByHandle(Binding.Value.OnReleasedHandle);
		}
	}

	// ���������ID��ƥ��ļ�������ID
	if (AbilityComponent)
	{
		for (auto& Binding : MappedAbilities) {
			const auto ActionInputID = Binding.Value.InputID;
			for (auto AbilityHandle : Binding.Value.BoundAbilitiesStack)
			{
				auto AbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
				if (AbilitySpec && ActionInputID == AbilitySpec->InputID)
				{
					AbilitySpec->InputID = ActionAbilityInputID::InvalidInputID;
				}
			}
		}
	}
	AbilityComponent = nullptr;
}

void UAbilitiesInputComponent::OnAbilityInputPressed(UInputAction* InputAction)
{
	// The AbilitySystemComponent may not have been valid when we first bound input... try again.
	if (!AbilityComponent)
	{
		SetupBindings();
	}

	if (AbilityComponent)
	{
		auto Binding = MappedAbilities.Find(InputAction);
		if (Binding && ensure(Binding->InputID != ActionAbilityInputID::InvalidInputID))
		{
			AbilityComponent->AbilityLocalInputPressed(Binding->InputID);
		}
	}
}

void UAbilitiesInputComponent::OnAbilityInputReleased(UInputAction* InputAction)
{
	// The AbilitySystemComponent may not have been valid when we first bound input... try again.
	if (!AbilityComponent)
	{
		SetupBindings();
	}

	if (AbilityComponent)
	{
		auto Binding = MappedAbilities.Find(InputAction);
		if (Binding && ensure(Binding->InputID != ActionAbilityInputID::InvalidInputID))
		{
			AbilityComponent->AbilityLocalInputReleased(Binding->InputID);
		}
	}
}

void UAbilitiesInputComponent::SetupBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle)
{
	if (!AbilityComponent)
	{
		return;
	}

	auto Binding = MappedAbilities.Find(InputAction);
	if (Binding)
	{
		// �����Ѵ���ʱ�ȷ���ջ������
		FGameplayAbilitySpec* BoundAbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(Binding->BoundAbilitiesStack.Top());
		if (BoundAbilitySpec && BoundAbilitySpec->InputID == Binding->InputID)
		{
			BoundAbilitySpec->InputID = ActionAbilityInputID::InvalidInputID;
		}
	}
	else
	{
		// ����������ʱ�����ò���������InputID
		Binding = &MappedAbilities.Add(InputAction);
		Binding->InputID = ActionAbilityInputID::GetNewInputID();
	}

	FGameplayAbilitySpec* AbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
	if (AbilitySpec)
	{
		// �����󶨼���Push��ջ��
		Binding->BoundAbilitiesStack.Push(AbilityHandle);

		// ͬ������ID
		AbilitySpec->InputID = Binding->InputID;

		// ͬ������������
		if (InputComponent)
		{
			if (Binding->OnPressedHandle == 0)
			{
				Binding->OnPressedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UAbilitiesInputComponent::OnAbilityInputPressed, InputAction).GetHandle();
			}

			if (Binding->OnReleasedHandle == 0)
			{
				Binding->OnReleasedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UAbilitiesInputComponent::OnAbilityInputReleased, InputAction).GetHandle();
			}
		}
	}
}

void UAbilitiesInputComponent::TeardownActionBinding(UInputAction* InputAction)
{
	if (!AbilityComponent)
	{
		return;
	}

	if (auto Binding = MappedAbilities.Find(InputAction))
	{
		// �Ƴ���ǿ�����������
		if (InputComponent)
		{
			InputComponent->RemoveBindingByHandle(Binding->OnPressedHandle);
			InputComponent->RemoveBindingByHandle(Binding->OnReleasedHandle);
		}

		// ���������ID��ƥ��ļ�������ID
		for (auto AbilityHandle : Binding->BoundAbilitiesStack)
		{
			auto AbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
			if (AbilitySpec && AbilitySpec->InputID == Binding->InputID)
			{
				AbilitySpec->InputID = ActionAbilityInputID::InvalidInputID;
			}
		}

		// �Ƴ��˲���
		MappedAbilities.Remove(InputAction);
	}
}

void UAbilitiesInputComponent::TeardownAbilityBinding(FGameplayAbilitySpecHandle AbilityHandle)
{
	if (!AbilityComponent)
	{
		return;
	}

	FGameplayAbilitySpec* AbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
	if (AbilitySpec)
	{
		auto MappedIterator = MappedAbilities.CreateIterator();
		while (MappedIterator)
		{
			if (MappedIterator.Value().InputID == AbilitySpec->InputID)
			{
				break;
			}
			++MappedIterator;
		}

		if (MappedIterator)
		{
			auto& Binding = MappedIterator.Value();
			if (Binding.BoundAbilitiesStack.Remove(AbilityHandle) > 0)
			{
				// ɾ�����ܰ󶨺��������InputID
				AbilitySpec->InputID = ActionAbilityInputID::InvalidInputID;

				if (Binding.BoundAbilitiesStack.Num())
				{
					// �������������������ԭջ��InputID
					FGameplayAbilitySpec* BoundAbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(Binding.BoundAbilitiesStack.Top());
					if (BoundAbilitySpec && BoundAbilitySpec->InputID == ActionAbilityInputID::InvalidInputID)
					{
						BoundAbilitySpec->InputID = Binding.InputID;
					}
				}
				else
				{
					// �Ƴ���ǿ�����������
					if (InputComponent)
					{
						InputComponent->RemoveBindingByHandle(Binding.OnPressedHandle);
						InputComponent->RemoveBindingByHandle(Binding.OnReleasedHandle);
					}

					// �Ƴ��˲���
					MappedAbilities.Remove(MappedIterator.Key());
				}
			}
		}
	}
}

void UAbilitiesInputComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	TeardownBindings();

	SetupBindings();

	// �����в�����Started/Completed�󶨵�Pressed/Released��
	// �ٸ��ݾ���InputID���ü���ϵͳ��Pressed/Released�ӿ�
	for (auto& Binding : MappedAbilities)
	{
		UInputAction* InputAction = Binding.Key;

		// Pressed event
		InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UAbilitiesInputComponent::OnAbilityInputPressed, InputAction);

		// Released event
		InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UAbilitiesInputComponent::OnAbilityInputReleased, InputAction);
	}
}

void UAbilitiesInputComponent::TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	TeardownBindings();
}
