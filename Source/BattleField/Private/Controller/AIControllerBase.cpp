// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIControllerBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "character/BattlefieldCharacterBase.h"

AAIControllerBase::AAIControllerBase()
{
	/* step1 : 创建组件 */
	/* 1.1 AI感知组件 */
	perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	if (perception) {
		/* 1.1.1 视觉组件 */
		sight_cfg = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISightConfig"));
		if (sight_cfg) {
			sight_cfg->SightRadius = 2000.f;
			sight_cfg->LoseSightRadius = 3000.f;
			sight_cfg->PeripheralVisionAngleDegrees = 70.f;
			sight_cfg->DetectionByAffiliation.bDetectEnemies = false;
			sight_cfg->DetectionByAffiliation.bDetectFriendlies = false;
			sight_cfg->DetectionByAffiliation.bDetectNeutrals = true;

			perception->ConfigureSense(*sight_cfg);
		}
	} else {
		return;
	}
	
	perception->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	perception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetPerceptionUpdated);
}

// Called when the game starts or when spawned
void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void AAIControllerBase::OnTargetPerceptionUpdated(AActor* actor, FAIStimulus stimulus)
{
	ABattlefieldCharacterBase* player = Cast<ABattlefieldCharacterBase>(actor);
	if (player) {
		UE_LOG(RunLog, Warning, TEXT("Perception Character"));
	}
}