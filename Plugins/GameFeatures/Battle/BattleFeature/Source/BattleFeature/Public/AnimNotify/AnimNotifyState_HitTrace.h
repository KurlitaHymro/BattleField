// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayEffect.h"
#include "Calculation/TableRowDefine.h"
#include "AnimNotifyState_HitTrace.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEFEATURE_API UAnimNotifyState_HitTrace : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName EquipSlot;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName HitPoint;

	UPROPERTY(EditAnywhere, Category = "Ability")
	FName HitMoveName;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultSelfMoveEffect;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	class ABattleCharacter* OwnerCharacter = nullptr;

	class UAbilitySystemComponent* OwnerASC = nullptr;

	class AMoveDamageWeapon* Weapon;

	float DamageFactor;

	FActiveGameplayEffectHandle Handle;

	FVector* HitPointInfo;

	FVector HitPointHalfSize;
	FVector HitPointLocation;

	TArray<AActor*> HitActors;

private:
	void UpdateHitResult(TArray<FHitResult> AllHitResult);
};
