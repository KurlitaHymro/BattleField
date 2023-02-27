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

	// 分配操作ID
	AbilityComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MyOwner);
	if (AbilityComponent)
	{
		for (auto& Binding : MappedAbilities)
		{
			const auto NewInputID = ActionAbilityInputID::GetNewInputID();
			Binding.Value.InputID = NewInputID;

			// 同步操作ID到技能输入ID
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
	// 移除增强输入操作代理
	if (InputComponent)
	{
		for (auto& Binding : MappedAbilities)
		{
			InputComponent->RemoveBindingByHandle(Binding.Value.OnPressedHandle);
			InputComponent->RemoveBindingByHandle(Binding.Value.OnReleasedHandle);
		}
	}

	// 废弃与操作ID所匹配的技能输入ID
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
		// 操作已存在时先废弃栈顶技能
		FGameplayAbilitySpec* BoundAbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(Binding->BoundAbilitiesStack.Top());
		if (BoundAbilitySpec && BoundAbilitySpec->InputID == Binding->InputID)
		{
			BoundAbilitySpec->InputID = ActionAbilityInputID::InvalidInputID;
		}
	}
	else
	{
		// 操作不存在时新增该操作并分配InputID
		Binding = &MappedAbilities.Add(InputAction);
		Binding->InputID = ActionAbilityInputID::GetNewInputID();
	}

	FGameplayAbilitySpec* AbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
	if (AbilitySpec)
	{
		// 将待绑定技能Push到栈顶
		Binding->BoundAbilitiesStack.Push(AbilityHandle);

		// 同步输入ID
		AbilitySpec->InputID = Binding->InputID;

		// 同步操作代理句柄
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
		// 移除增强输入操作代理
		if (InputComponent)
		{
			InputComponent->RemoveBindingByHandle(Binding->OnPressedHandle);
			InputComponent->RemoveBindingByHandle(Binding->OnReleasedHandle);
		}

		// 废弃与操作ID所匹配的技能输入ID
		for (auto AbilityHandle : Binding->BoundAbilitiesStack)
		{
			auto AbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle);
			if (AbilitySpec && AbilitySpec->InputID == Binding->InputID)
			{
				AbilitySpec->InputID = ActionAbilityInputID::InvalidInputID;
			}
		}

		// 移除此操作
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
				// 删除技能绑定后废弃技能InputID
				AbilitySpec->InputID = ActionAbilityInputID::InvalidInputID;

				if (Binding.BoundAbilitiesStack.Num())
				{
					// 如果仍留有其他技能则复原栈顶InputID
					FGameplayAbilitySpec* BoundAbilitySpec = AbilityComponent->FindAbilitySpecFromHandle(Binding.BoundAbilitiesStack.Top());
					if (BoundAbilitySpec && BoundAbilitySpec->InputID == ActionAbilityInputID::InvalidInputID)
					{
						BoundAbilitySpec->InputID = Binding.InputID;
					}
				}
				else
				{
					// 移除增强输入操作代理
					if (InputComponent)
					{
						InputComponent->RemoveBindingByHandle(Binding.OnPressedHandle);
						InputComponent->RemoveBindingByHandle(Binding.OnReleasedHandle);
					}

					// 移除此操作
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

	// 将所有操作的Started/Completed绑定到Pressed/Released上
	// 再根据具体InputID调用技能系统的Pressed/Released接口
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
