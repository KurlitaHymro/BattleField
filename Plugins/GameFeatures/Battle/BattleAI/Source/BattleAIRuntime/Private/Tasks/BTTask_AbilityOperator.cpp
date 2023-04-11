// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTTask_AbilityOperator.h"
#include "BattleAbilitySystemComponent.h"
#include "AIController.h"
//#include "BattleCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_AbilityOperator)

UBTTask_AbilityOperator::UBTTask_AbilityOperator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Ability Operator";
	// instantiating to be able to use Timers
	bCreateNodeInstance = true;

	FGameplayAbilitySpec AbilitySpec(AbilityType.LoadSynchronous());
}

EBTNodeResult::Type UBTTask_AbilityOperator::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const MyController = OwnerComp.GetAIOwner();
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	MyOwnerComp = &OwnerComp;
	if (AbilityType && MyController && MyController->GetPawn())
	{
		//ACharacter* const MyCharacter = Cast<ACharacter>(MyController->GetPawn());

	}

	return EBTNodeResult::Type();
}

EBTNodeResult::Type UBTTask_AbilityOperator::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

FString UBTTask_AbilityOperator::GetStaticDescription() const
{
	return FString();
}




#if WITH_EDITOR
FName UBTTask_AbilityOperator::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.PlaySound.Icon");
}
#endif	// WITH_EDITOR