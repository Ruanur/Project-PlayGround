// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/HUD/Playground_StatsDisplay.h"
#include "AbilitySystemComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystem/PlaygroundAttributeSet.h"

void UPlayground_StatsDisplay::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Text_StatInfo)
	{
		Text_StatInfo->SetText(StatInfoText);
	}
}

void UPlayground_StatsDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	if (UAbilitySystemComponent* ASC = ResolveASC())
	{
		BindToASC(ASC);
		RefreshText();
	}
}

void UPlayground_StatsDisplay::NativeDestruct()
{
	UnbindFromASC();
	Super::NativeDestruct();
}

UAbilitySystemComponent* UPlayground_StatsDisplay::ResolveASC() const
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC) return nullptr;

	APawn* Pawn = PC->GetPawn();
	if (Pawn)
	{
		if (UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>())
		{
			return ASC;
		}
	}

	if (APlayerState* PS = PC->PlayerState)
	{
		if (UAbilitySystemComponent* ASC = PS->FindComponentByClass<UAbilitySystemComponent>())
		{
			return ASC;
		}
	}

	return nullptr;
}

FGameplayAttribute UPlayground_StatsDisplay::GetAttributeForField(EPlaygroundStatField Field) const
{
	switch (Field)
	{
	case EPlaygroundStatField::CurrentHealth:
		return UPlaygroundAttributeSet::GetCurrentHealthAttribute();
	case EPlaygroundStatField::MaxHealth:
		return UPlaygroundAttributeSet::GetMaxHealthAttribute();;
	case EPlaygroundStatField::CurrentRage:
		return UPlaygroundAttributeSet::GetCurrentRageAttribute();;
	case EPlaygroundStatField::MaxRage:
		return UPlaygroundAttributeSet::GetMaxRageAttribute();;
	case EPlaygroundStatField::AttackPower:
		return UPlaygroundAttributeSet::GetAttackPowerAttribute();;
	case EPlaygroundStatField::DefensePower:
		return UPlaygroundAttributeSet::GetDefensePowerAttribute();
	case EPlaygroundStatField::BaseDamage:
		return UPlaygroundAttributeSet::GetBaseDamageAttribute();
	default:
		return FGameplayAttribute();
	}

}

FGameplayAttribute UPlayground_StatsDisplay::GetSecondaryAttributeIfNeeded(EPlaygroundStatField Field) const
{
	return FGameplayAttribute();
}

void UPlayground_StatsDisplay::BindToASC(UAbilitySystemComponent* ASC)
{
	if (!ASC) return;
	CachedASC = ASC;

	const FGameplayAttribute PrimaryAttr = GetAttributeForField(StatField);
	if (PrimaryAttr.IsValid())
	{
		PrimaryHandle = ASC->GetGameplayAttributeValueChangeDelegate(PrimaryAttr).AddUObject(this, &ThisClass::OnPrimaryChanged);
	}

	const FGameplayAttribute SecondaryAttr = GetSecondaryAttributeIfNeeded(StatField);
	if (SecondaryAttr.IsValid())
	{
		SecondaryHandle = ASC->GetGameplayAttributeValueChangeDelegate(SecondaryAttr).AddUObject(this, &ThisClass::OnSecondaryChanged);
	}
}

void UPlayground_StatsDisplay::UnbindFromASC()
{
	if (!CachedASC.IsValid()) return;

	const FGameplayAttribute PrimaryAttr = GetAttributeForField(StatField);
	if (PrimaryAttr.IsValid())
	{
		CachedASC->GetGameplayAttributeValueChangeDelegate(PrimaryAttr).Remove(PrimaryHandle);
	}

	const FGameplayAttribute SecondaryAttr = GetSecondaryAttributeIfNeeded(StatField);
	if (SecondaryAttr.IsValid())
	{
		CachedASC->GetGameplayAttributeValueChangeDelegate(SecondaryAttr).Remove(SecondaryHandle);
	}

	PrimaryHandle.Reset();
	SecondaryHandle.Reset();
	CachedASC = nullptr;
}

void UPlayground_StatsDisplay::RefreshText()
{
	if (!Text_Value || !CachedASC.IsValid()) return;

	const FGameplayAttribute Attr = GetAttributeForField(StatField);
	if (!Attr.IsValid()) return;

	const float Value = CachedASC->GetNumericAttribute(Attr);

	FNumberFormattingOptions Opt;
	Opt.MinimumFractionalDigits = FractionDigits;
	Opt.MaximumFractionalDigits = FractionDigits;

	Text_Value->SetText(FText::AsNumber(Value, &Opt));
}

void UPlayground_StatsDisplay::OnPrimaryChanged(const FOnAttributeChangeData& Data)
{
	RefreshText();
}

void UPlayground_StatsDisplay::OnSecondaryChanged(const FOnAttributeChangeData& Data)
{
	RefreshText();
}



