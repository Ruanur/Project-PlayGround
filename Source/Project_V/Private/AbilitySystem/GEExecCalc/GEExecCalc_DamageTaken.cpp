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



