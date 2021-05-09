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
			sight_cfg->SightRadius = 2000.f;
			sight_cfg->LoseSightRadius = 3000.f;
			sight_cfg->PeripheralVisionAngleDegrees = 70.f;
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
	ai->PawnDead.AddUniqueDynamic(this, &AAIControllerBase::OnPawnDead);
}

void AAIControllerBase::OnTargetPerceptionUpdated(AActor* actor, FAIStimulus stimulus)
{
	ABattlefieldCharacterBase* target = Cast<ABattlefieldCharacterBase>(actor);
	ABattlefieldCharacterBase* ctrlPawn = Cast<ABattlefieldCharacterBase>(GetPawn());
	if (target && ctrlPawn) {
		if (target->bIsEnemy != ctrlPawn->bIsEnemy) {
			if (stimulus.IsValid()) {
				UE_LOG(RunLog, Error, TEXT("Push : %d"), TargetEnemy.Num());
				TargetEnemy.AddUnique(target);
			}
		}
	}
}

void AAIControllerBase::OnPawnDead()
{
	ABattlefieldCharacterAI* ai = Cast<ABattlefieldCharacterAI>(GetPawn());
	if (ai) {
		ai->DetachFromControllerPendingDestroy();
		ai->Dead();
	}
}