// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/GameplayAbility_MeleeMove.h"
#include "BattleCharacter.h"
#include "AbilitySystemComponent.h"

FGameplayAbilityTargetDataHandle UGameplayAbility_MeleeMove::AssembleTargetData(const FGameplayEventData& EventData)
{
	AActor* OwnerActor = GetOwningActorFromActorInfo();
	ABattleCharacter* OwnerCharacter = Cast<ABattleCharacter>(OwnerActor);
	UAbilitySystemComponent* InstigatorASC = OwnerCharacter->GetAbilitySystemComponent();

	if (InstigatorASC)
	{
		FGameplayAbilityTargetDataHandle TargetData;

		AActor* Target = Cast<AActor>(EventData.Target);

		FGameplayAbilityTargetData_ActorArray* HitActorArrayData = new FGameplayAbilityTargetData_ActorArray();
		HitActorArrayData->TargetActorArray.Add(Target);

		TargetData.Add(HitActorArrayData);
		return TargetData;
	}

	return FGameplayAbilityTargetDataHandle();
}