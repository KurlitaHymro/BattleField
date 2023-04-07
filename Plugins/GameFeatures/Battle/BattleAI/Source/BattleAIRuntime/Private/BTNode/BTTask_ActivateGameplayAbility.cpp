// Fill out your copyright notice in the Description page of Project Settings.


#include "BTNode/BTTask_ActivateGameplayAbility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BfCharacter.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UBTTask_ActivateGameplayAbility::UBTTask_ActivateGameplayAbility()
{
	NodeName = "TryActivateAbility";
	bNotifyTick = true;
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ActivateGameplayAbility, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::TypeUBTTask_ActivateGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerPawn = Cast<ABfCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AbilitySystemComponent = OwnerPawn->GetAbilitySystemComponent();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AbilitySystemComponent.
	return EBTNodeResult::Succeeded;
}

void UBTTask_ActivateGameplayAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
