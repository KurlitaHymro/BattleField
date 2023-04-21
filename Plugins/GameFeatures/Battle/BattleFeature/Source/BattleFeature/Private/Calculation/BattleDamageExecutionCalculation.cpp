// Fill out your copyright notice in the Description page of Project Settings.


#include "Calculation/BattleDamageExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AttackAttributeSet)

struct BattleDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);

	BattleDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttackAttributeSet, AttackPower, Source, true);
	}
};

static const BattleDamageStatics& DamageStatics()
{
	static BattleDamageStatics DamageStatics;
	return DamageStatics;
}

UBattleDamageExecutionCalculation::UBattleDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
}

void UBattleDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// --------------------------------------
	//	CauseDamage = MeleeMoveFactos * AttackPower
	//	ApplyCauseDamage = CauseDamage - DefensivePower
	// --------------------------------------

	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);

	float Damage = AttackPower;
	if (Damage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().AttackPowerProperty, EGameplayModOp::Additive, Damage));
	}
}