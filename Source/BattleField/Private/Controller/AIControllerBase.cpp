// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIControllerBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "character/BattlefieldCharacterBase.h"
#include "character/BattlefieldCharacterAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAIControllerBase::AAIControllerBase()
{
	/* step1 : 创建组件 */
	/* 1.1 AI感知组件 */
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	if (PerceptionComp) {
		/* 1.1.1 视觉组件 */
		sight_cfg = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISightConfig"));
		if (sight_cfg) {
			sight_cfg->SightRadius = 800.f;
			sight_cfg->LoseSightRadius = 1200.f;
			sight_cfg->PeripheralVisionAngleDegrees = 180.f;
			sight_cfg->DetectionByAffiliation.bDetectEnemies = false;
			sight_cfg->DetectionByAffiliation.bDetectFriendlies = false;
			sight_cfg->DetectionByAffiliation.bDetectNeutrals = true;

			PerceptionComp->ConfigureSense(*sight_cfg);
		}
	} else {
		return;
	}
	PerceptionComp->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetPerceptionUpdated);
	
	/* 1.2 黑板组件 */
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	/* 1.2 行为树组件 */
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	bWantsPlayerState = true;
}

// Called when the game starts or when spawned
void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void AAIControllerBase::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABattlefieldCharacterAI* ai = Cast<ABattlefieldCharacterAI>(InPawn);
	/* 加载行为树和黑板数据 */
	if (ai && ai->BehaviorTree && BlackboardComp && BehaviorTreeComp) {
		if (ai->BehaviorTree->BlackboardAsset) {
			BlackboardComp->InitializeBlackboard(*ai->BehaviorTree->BlackboardAsset);
		}
		BehaviorTreeComp->StartTree(*ai->BehaviorTree);
	} else {
		UE_LOG(RunLog, Error, TEXT("Init BehaviorTree Fail"));
	}
	/* 挂载角色死亡代理 */
	ai->PawnDead.AddDynamic(this, &AAIControllerBase::OnCtrlPawnDead);
}

void AAIControllerBase::OnTargetPerceptionUpdated(AActor* actor, FAIStimulus stimulus)
{
	ABattlefieldCharacterBase* target = Cast<ABattlefieldCharacterBase>(actor);
	ABattlefieldCharacterBase* ctrlPawn = Cast<ABattlefieldCharacterBase>(GetPawn());

	if (stimulus.WasSuccessfullySensed()) {
		UE_LOG(RunLog, Error, TEXT("PerceptionUpdated %s ---> %s  Cur : %d"),
			*GetDebugName(this), *GetDebugName(target->Controller), TargetEnemy.Num());
	} else {
		UE_LOG(RunLog, Error, TEXT("PerceptionUpdated %s -|-> %s  Cur : %d"),
			*GetDebugName(this), *GetDebugName(target->Controller), TargetEnemy.Num());
	}

	if (ctrlPawn && target && target->bIsValid) {
		if (target->bIsEnemy != ctrlPawn->bIsEnemy) {
			if (stimulus.IsValid()) {
				if (stimulus.WasSuccessfullySensed()) {
					TargetEnemy.AddUnique(target);
					/* 挂载目标行为代理 */
					target->TargetBehavior.AddDynamic(this, &AAIControllerBase::OnTargetBehavior);
				} else if (TargetEnemy.Num() > 3) {
					/* 如果目标个数充裕则放弃暂时丢失的目标 */
					TargetEnemy.Remove(target);
				}
				UpdateBlackboard();
			}
		}
	}
}

void AAIControllerBase::OnCtrlPawnDead()
{
	ABattlefieldCharacterAI* ai = Cast<ABattlefieldCharacterAI>(GetPawn());
	if (ai) {
		ai->DetachFromControllerPendingDestroy();
		ai->Dead();
	}
}

void AAIControllerBase::OnTargetBehavior(ABattlefieldCharacterBase* Target, EnumCharacterTargetBehavior Behavior)
{
	switch (Behavior) {
	case EnumCharacterTargetBehavior::EN_DEAD:
		Target->TargetBehavior.RemoveDynamic(this, &AAIControllerBase::OnTargetBehavior);
		TargetEnemy.Remove(Target);
		break;
	}
	UpdateBlackboard();
}

void AAIControllerBase::UpdateBlackboard()
{
	if (!BlackboardComp) {
		return;
	}

	AActor* FirstPriorityTarget = GetFirstPriorityTarget();

	BlackboardComp->SetValueAsInt("EnemyNum", TargetEnemy.Num());
	if (!FirstPriorityTarget) {
		BlackboardComp->SetValueAsObject("EnemyActor", nullptr);
		BlackboardComp->SetValueAsBool("HasLineOfSight", false);
	} else {
		BlackboardComp->SetValueAsObject("EnemyActor", FirstPriorityTarget);
		BlackboardComp->SetValueAsBool("HasLineOfSight", true);
	}
}

AActor* AAIControllerBase::GetFirstPriorityTarget()
{
	APawn* p = GetPawn();
	AActor* nearestTarget = nullptr;
	if (TargetEnemy.Num() > 0 && p) {
		float minDistance = 1000.f;
		for (auto It = TargetEnemy.CreateConstIterator(); It; ++It) {
			float distance = p->GetDistanceTo(*It);
			if (distance < minDistance) {
				minDistance = distance;
				nearestTarget = *It;
			}
		}
	}
	return nearestTarget;
}