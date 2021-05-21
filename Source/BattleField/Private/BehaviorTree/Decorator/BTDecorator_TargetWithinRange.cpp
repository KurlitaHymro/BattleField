// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Decorator/BTDecorator_TargetWithinRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

UBTDecorator_TargetWithinRange::UBTDecorator_TargetWithinRange()
{
	NodeName = "TargetWithinRange";

	// aborting child nodes doesn't makes sense, cooldown starts after leaving this branch
	bAllowAbortChildNodes = false;

	bNotifyTick = false;
	bNotifyDeactivation = true;

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_TargetWithinRange, BlackboardKey), AActor::StaticClass());
}

bool UBTDecorator_TargetWithinRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerController && MyBlackboard) {
		AActor* target = Cast<AActor>(MyBlackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));
		APawn* selfPawn = OwnerController->GetPawn();
		if (selfPawn && target) {
			// Distance
			float distance = selfPawn->GetDistanceTo(target);

			// AngleDegrees
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
				selfPawn->GetActorLocation(),
				target->GetActorLocation());

			float AbsYawDiff = UKismetMathLibrary::Abs(
				UKismetMathLibrary::NormalizedDeltaRotator(
					LookAtRotation,
					selfPawn->GetActorRotation()
				).Yaw);

			if (distance < RangeDistance && AbsYawDiff < AngleDegrees) {
				return true;
			}
		}
	}
	return false;
}
