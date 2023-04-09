// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTTask_AbilityOperator.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_AbilityOperator)

UBTTask_AbilityOperator::UBTTask_AbilityOperator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Ability Operator";
	// instantiating to be able to use Timers
	bCreateNodeInstance = true;

	FGameplayAbilitySpec AbilitySpec(AbilityType.LoadSynchronous());
}


#if WITH_EDITOR
FName UBTTask_AbilityOperator::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.PlaySound.Icon");
}
#endif	// WITH_EDITOR