// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayAbility_Combo.h"
#include "AbilitiesInputComponent.h"
#include "AbilitySystemComponent.h"
#include "GrantAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_Combo)

void UGameplayAbility_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bIsActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Succ To Play %s"), *MontageToPlay.GetName());
		UpdateDervieAbilities();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail To Play %s"), *MontageToPlay.GetName());
		K2_EndAbility();
	}
}



void UGameplayAbility_Combo::OnCancelled()
{
	OnTaskEnd();

	K2_EndAbility();
}

void UGameplayAbility_Combo::OnInterrupted()
{
	OnTaskEnd();

	K2_EndAbility();
}

void UGameplayAbility_Combo::OnBlendOut()
{
	OnTaskEnd();
}

void UGameplayAbility_Combo::OnCompleted()
{
	K2_EndAbility();
}

void UGameplayAbility_Combo::UpdateDervieAbilities()
{
	UGrantAbilitySystemComponent* GASC = Cast<UGrantAbilitySystemComponent>(AbilitySystemComponent);
	if (GASC == nullptr || AbilitiesInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent or AbilitiesInputComponent is null"));
		return;
	}

	// 成功释放一个技能后，将另一个技能绑定到操作栈上。
	for (auto Dervie : DervieAbilities)
	{
		FGameplayAbilitySpecHandle* Handle = GASC->GrantedAbilitiesHandle.Find(Dervie.Value);
		if (Handle != nullptr && Handle->IsValid())
		{
			AbilitiesInputComponent->SetupBinding(Dervie.Key, *Handle);
		}
	}
}