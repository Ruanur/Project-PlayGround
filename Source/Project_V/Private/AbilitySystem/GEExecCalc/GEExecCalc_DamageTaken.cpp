// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "PlaygroundGameplayTags.h"
#include "AbilitySystem/PlaygroundAttributeSet.h"

#include "PlaygroundDebugHelper.h"

struct FPlaygroundDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage)

	FPlaygroundDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlaygroundAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlaygroundAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlaygroundAttributeSet, DamageTaken, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlaygroundAttributeSet, BaseDamage, Source, false)
	}
};

static const FPlaygroundDamageCapture& GetPlaygroundDamageCapture()
{
	static FPlaygroundDamageCapture PlaygroundDamageCapture;
	return PlaygroundDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	//캡처 느림
 
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
	RelevantAttributesToCapture.Add(GetPlaygroundDamageCapture().DamageTakenDef);
	RelevantAttributesToCapture.Add(GetPlaygroundDamageCapture().BaseDamageDef);
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

	// ===================
	// 캡처 : 공격력 / 방어력 / 기본피해
	// ===================
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPlaygroundDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	//Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPlaygroundDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	//Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);	

	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPlaygroundDamageCapture().BaseDamageDef, EvaluateParameters, BaseDamage);

	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(PlaygroundGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			//Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}
		
		if (TagMagnitude.Key.MatchesTagExact(PlaygroundGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedLightAttackComboCount"), UsedLightAttackComboCount);
		}

		if (TagMagnitude.Key.MatchesTagExact(PlaygroundGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedHeavyAttackComboCount"), UsedHeavyAttackComboCount);
		}
	}


	//피해 계산식
	// ================= 
	// 콤보 배율
	// =================
	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05f + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
		//Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);
	}
	
	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
		//Debug::Print(TEXT("ScaledBaseDamageHeavy"), BaseDamage);
	}

	// =============
	// 최종 데미지
	// =============
	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	Debug::Print(TEXT("BaseDamge"), BaseDamage);

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetPlaygroundDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}



