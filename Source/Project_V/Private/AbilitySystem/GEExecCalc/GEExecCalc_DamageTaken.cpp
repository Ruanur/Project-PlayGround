// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "PlaygroundGameplayTags.h"
#include "AbilitySystem/PlaygroundAttributeSet.h"

struct FPlaygroundDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)

	FPlaygroundDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlaygroundAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlaygroundAttributeSet, DefensePower, Target, false)
	}
};

static const FPlaygroundDamageCapture& GetPlaygroundDamageCapture()
{
	static FPlaygroundDamageCapture PlaygroundDamageCapture;
	return PlaygroundDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	//Ä¸Ã³ ´À¸²
 
	//FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	//	UPlaygroundAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(UPlaygroundAttributeSet, AttackPower)
	//);

	//FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	//	AttackPowerProperty,
	//	EGameplayEffectAttributeCaptureSource::Source,
	//	false
	//);

	//RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

	RelevantAttributesToCapture.Add(GetPlaygroundDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetPlaygroundDamageCapture().DefensePowerDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	//EffectSpec.GetContext().GetSourceObject();
	//EffectSpec.GetContext().GetAbility();
	//EffectSpec.GetContext().GetInstigator();
	//EffectSpec.GetContext().GetEffectCauser();


	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPlaygroundDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);

	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(PlaygroundGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
		
		if (TagMagnitude.Key.MatchesTagExact(PlaygroundGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(PlaygroundGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
		}
	}

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPlaygroundDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
}



