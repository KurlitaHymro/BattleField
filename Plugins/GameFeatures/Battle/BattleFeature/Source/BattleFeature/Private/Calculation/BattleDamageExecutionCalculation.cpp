// Fill out your copyright notice in the Description page of Project Settings.


#include "Calculation/BattleDamageExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AttackAttributeSet)
#include UE_INLINE_GENERATED_CPP_BY_NAME(DefenseAttributeSet)
#include UE_INLINE_GENERATED_CPP_BY_NAME(HealthAttributeSet)

// -------------------------------------------------------------------------
//	Helper macros for declaring attribute captures 
// -------------------------------------------------------------------------

#define DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(P, T) \
	FProperty* T##P##Property; \
	FGameplayEffectAttributeCaptureDefinition T##P##Def; \

#define DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(S, P, T, B) \
{ \
	T##P##Property = FindFieldChecked<FProperty>(S::StaticClass(), GET_MEMBER_NAME_CHECKED(S, P)); \
	T##P##Def = FGameplayEffectAttributeCaptureDefinition(T##P##Property, EGameplayEffectAttributeCaptureSource::T, B); \
}

struct BattleDamageStatics
{
	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(AttackPower, Source);
	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(MeleeMoveFactor, Source);

	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(DefensePower, Target);
	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(Health, Target);

	BattleDamageStatics()
	{
		// 伤害来源的属性
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UAttackAttributeSet, AttackPower, Source, true);
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UAttackAttributeSet, MeleeMoveFactor, Source, true);

		// 伤害目标的属性
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UDefenseAttributeSet, DefensePower, Target, true);
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, Health, Target, true);
	}
};

static const BattleDamageStatics& DamageStatics()
{
	static BattleDamageStatics DamageStatics;
	return DamageStatics;
}

UBattleDamageExecutionCalculation::UBattleDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().SourceAttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().SourceMeleeMoveFactorDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetDefensePowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().TargetHealthDef);
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
	//	ApplyCauseDamage = CauseDamage * (DefensePower / (DefensePower + 100))
	// --------------------------------------

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceAttackPowerDef, EvaluationParameters, SourceAttackPower);

	float SourceMeleeMoveFactor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceMeleeMoveFactorDef, EvaluationParameters, SourceMeleeMoveFactor);

	float CauseDamage = SourceAttackPower * SourceMeleeMoveFactor;

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetDefensePowerDef, EvaluationParameters, TargetDefensePower);

	float ApplyDamage = CauseDamage * (TargetDefensePower / (TargetDefensePower + 100.f));
	if (ApplyDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().TargetHealthProperty, EGameplayModOp::Additive, -ApplyDamage));
	}
}