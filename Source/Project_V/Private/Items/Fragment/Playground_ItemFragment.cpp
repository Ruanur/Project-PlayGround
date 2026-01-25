// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Fragment/Playground_ItemFragment.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "Widgets/Composite/Playground_CompositeBase.h"
#include "Widgets/Composite/Playground_LeafImage.h"
#include "Widgets/Composite/Playground_LeafText.h"
#include "Widgets/Composite/Playground_LeafLabeledValue.h"
#include "AbilitySystem/PlaygroundAttributeSet.h"
#include "GameplayEffect.h"

#include "PlaygroundDebugHelper.h"

void FPlayground_InventoryItemFragment::Assimilate(UPlayground_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->PG_Expand();
}

void FPlayground_TextFragment::Assimilate(UPlayground_CompositeBase* Composite) const
{
	FPlayground_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UPlayground_LeafText* LeafText = Cast<UPlayground_LeafText>(Composite);
	if (!IsValid(LeafText)) return;

	LeafText->PG_SetText(FragmentText);
}

bool FPlayground_InventoryItemFragment::MatchesWidgetTag(const UPlayground_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

// Get a Stats component from the PC or the PC->GetPawn()
// or get the Ability System Component and apply a Gameplay Effect
// or call an interface function for Healing()
void FPlayground_HealthPotionFragment::OnConsume(APlayerController* PC, UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	if (!PC) return;

	APawn* Pawn = PC->GetPawn();
	if (!Pawn) return;

	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC) return;

	const UGameplayEffect* EffectCDO =
		ConsumableGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	ASC->ApplyGameplayEffectToSelf(
		EffectCDO,
		1,
		ASC->MakeEffectContext()
	);
	
	Debug::Print(TEXT("Health Potion consumed %f"), FColor::Green);
}

void FPlayground_ManaPotionFragment::OnConsume(APlayerController* PC, UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	// Replenish mana however you wish

	Debug::Print(TEXT("Mana Potion consumed %f"), FColor::Blue, ManaAmount);
}

void FPlayground_ImageFragment::Assimilate(UPlayground_CompositeBase* Composite) const
{
	FPlayground_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UPlayground_LeafImage* Image = Cast<UPlayground_LeafImage>(Composite);
	if (!IsValid(Image)) return;

	Image->SetImage(Icon);
	Image->SetBoxSize(IconDimensions);
	Image->SetImageSize(IconDimensions);

}

void FPlayground_LabeledNumberFragment::Assimilate(UPlayground_CompositeBase* Composite) const
{
	FPlayground_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UPlayground_LeafLabeledValue* LabeledValue = Cast<UPlayground_LeafLabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;

	LabeledValue->SetText_Label(Text_Label, bCollapseLabel);

	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;

	LabeledValue->SetText_Value(FText::AsNumber(Value, &Options), bCollapseValue);
	
}

void FPlayground_LabeledNumberFragment::Manifest()
{
	FPlayground_InventoryItemFragment::Manifest();

	if (!SourceGameplayEffect) return;

	const UGameplayEffect* GECDO = SourceGameplayEffect->GetDefaultObject<UGameplayEffect>();

	const FGameplayModifierInfo& Modifier = GECDO->Modifiers[0];

	float OutValue = 0.f;

	if (Modifier.ModifierMagnitude.AttemptCalculateMagnitude(
		FGameplayEffectSpec(),
		OutValue))
	{
		Value = OutValue;
	}

	//if (bRandomizeOnManifest)
	//{
	//	Value = FMath::FRandRange(Min, Max);
	//}
 
	//bRandomizeOnManifest = false;
}
