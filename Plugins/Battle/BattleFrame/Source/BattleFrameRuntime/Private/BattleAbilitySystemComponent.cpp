// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

namespace BattleAbilitySystemComponent_Impl
{
	constexpr int32 InvalidID = 0;
	int32 IncrementingID = InvalidID;

	static int32 GetNextID()
	{
		return ++IncrementingID;
	}
}

int32 UBattleAbilitySystemComponent::SynchronousLoadAbility(TSoftClassPtr<UGameplayAbility> AbilityType)
{
	FGameplayAbilitySpec AbilitySpec(AbilityType.LoadSynchronous());
	int32 newID = BattleAbilitySystemComponent_Impl::GetNextID();
	AbilitySpec.InputID = newID;
	FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(AbilitySpec);

	FBattleAbilityEntry* Entry = &MappedAbilities.Add(newID);
	Entry->AbilityType = AbilityType;
	Entry->AbilitySpec = AbilitySpec;
	Entry->AbilitySpecHandle = AbilityHandle;

	return newID;
}

void UBattleAbilitySystemComponent::RemoveAbility(int32 AbilityID)
{
	FBattleAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		SetRemoveAbilityOnEnd(Ability->AbilitySpecHandle);
		MappedAbilities.Remove(AbilityID);
	}
}

void UBattleAbilitySystemComponent::EnableAbility(int32 AbilityID)
{
	FBattleAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		Ability->AbilitySpec.InputID = AbilityID;
	}
}

void UBattleAbilitySystemComponent::DisableAbility(int32 AbilityID)
{
	FBattleAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		Ability->AbilitySpec.InputID = BattleAbilitySystemComponent_Impl::InvalidID;
	}
}

int32 UBattleAbilitySystemComponent::FindAbilityByType(TSoftClassPtr<UGameplayAbility> AbilityType)
{
	auto MappedIterator = MappedAbilities.CreateIterator();
	while (MappedIterator)
	{
		if (MappedIterator.Value().AbilityType == AbilityType)
		{
			return MappedIterator.Key();
		}
		++MappedIterator;
	}
	return BattleAbilitySystemComponent_Impl::InvalidID;
}

void UBattleAbilitySystemComponent::HandleHitEvent(AActor* Target)
{
	AActor* Instigator = GetOwner();
	auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (IsValid(Instigator) && IsValid(Target) && IsValidChecked(this) && IsValidChecked(TargetASC))
	{
		FGameplayEventData Payload;
		Payload.Instigator = Instigator;
		Payload.Target = Target;

		FVector DamageSourceDirection = Instigator->GetActorLocation() - Target->GetActorLocation();
		FVector ForwardVector = Target->GetActorForwardVector();
		const float AngleDifference = DamageSourceDirection.CosineAngle2D(ForwardVector);
		//UE_LOG(LogTemp, Error, TEXT("CosineAngleDifference %f"), AngleDifference);

		const float ShieldDefauseThresholdCosineAngle = 0.5f; // 60бу
		if (AngleDifference > ShieldDefauseThresholdCosineAngle && TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Battle.Base.Status.ShieldDefense"))))
		{
			HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Battle.Base.Event.HitShieldDefense")), &Payload);
		}
		else
		{
			HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Battle.Base.Event.HitDefault")), &Payload);
		}
	}
}



