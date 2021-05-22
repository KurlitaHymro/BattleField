// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CharacterAction.h"
#include "Character/BattlefieldCharacterBase.h"
#include "Animation/AnimInstance.h"

// Sets default values for this component's properties
UCharacterAction::UCharacterAction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	DeadAnmi = LoadObject<UAnimMontage>(NULL,
		TEXT("AnimBlueprint'/Game/Animations/Montage/DeadMontage.DeadMontage'"));

	NormalAttackCombo = LoadObject<UAnimMontage>(NULL,
		TEXT("AnimBlueprint'/Game/Animations/Montage/Combo_SzFhSlXp.Combo_SzFhSlXp'"));
	if (NormalAttackCombo) {
		NormalAttackComboNum = NormalAttackCombo->CompositeSections.Num();
		NormalAttackComboIdx = 0;
	} else {
		UE_LOG(LoadLog, Error, TEXT("Load Montage Error"));
	}
}


// Called when the game starts
void UCharacterAction::BeginPlay()
{
	Super::BeginPlay();

	if (NormalAttackCombo) {
		owner = Cast<ABattlefieldCharacterBase>(GetOwner());
		AnimInstance = owner->GetMesh()->GetAnimInstance();
		if (AnimInstance) {
			AnimInstance->OnMontageBlendingOut.AddDynamic(this, &UCharacterAction::OnMontageBlendingOut);
			AnimInstance->OnMontageStarted.AddDynamic(this, &UCharacterAction::OnMontageStarted);
			AnimInstance->OnMontageEnded.AddDynamic(this, &UCharacterAction::OnMontageEnded);
		} else {
			UE_LOG(LoadLog, Error, TEXT("Load AnimInstance Error"));
		}
	}
}

void UCharacterAction::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (saveInput && NormalAttackComboIdx) {
		float Duration = AnimInstance->Montage_Play(NormalAttackCombo, 1.f);
		FName SectionName = NormalAttackCombo->
			CompositeSections[NormalAttackComboIdx].SectionName;
		if (SectionName != NAME_None) {
			AnimInstance->Montage_JumpToSection(SectionName, NormalAttackCombo);
		}
		NormalAttackComboIdx = (NormalAttackComboIdx + 1) % NormalAttackComboNum;
		saveInput = 0;
	}
}

void UCharacterAction::OnMontageStarted(UAnimMontage* Montage)
{
	if (owner) {
		owner->bIsInMotion = true;
	}
}

void UCharacterAction::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	
	if (owner && !bInterrupted) {
		owner->bIsInMotion = false;
	}
}

// Called every frame
void UCharacterAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCharacterAction::Dead()
{
	if (!DeadAnmi) {
		UE_LOG(RunLog, Error, TEXT("Dead Montage Error"));
		return;
	}

	AnimInstance->Montage_Stop(0.f, nullptr);
	AnimInstance->Montage_Play(DeadAnmi, 1.4f);
}

void UCharacterAction::MainNormalAttack()
{
	if (!NormalAttackCombo || NormalAttackComboNum == 0) {
		UE_LOG(RunLog, Error, TEXT("NormalAttack Montage Error"));
		return;
	}

	if (AnimInstance->Montage_IsPlaying(NormalAttackCombo)) {
		saveInput = 1;
	} else {
		NormalAttackComboIdx = 0;
		float Duration = AnimInstance->Montage_Play(NormalAttackCombo, 0.9f);
		NormalAttackComboIdx++;
	}
}

