// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Composite/Playground_RarityBorderLeaf.h"
#include "Components/Image.h"

void UPlayground_RarityBorderLeaf::SetBorderByRarity(EPlaygroundRarity InRarity) const
{
	if (!IsValid(Border_Image))
	{
		return;
	}

	Border_Image->SetBrushTintColor(FSlateColor(GetRarityColor(InRarity)));
}

FLinearColor UPlayground_RarityBorderLeaf::GetRarityColor(EPlaygroundRarity InRarity) const
{
	switch (InRarity)
	{
	case EPlaygroundRarity::Common:
		return FLinearColor(0.60f, 0.60f, 0.60f, 1.0f);
	case EPlaygroundRarity::Uncommon:
		return FLinearColor(0.10f, 0.80f, 0.20f, 1.0f);
	case EPlaygroundRarity::Rare:
		return FLinearColor(0.10f, 0.40f, 1.00f, 1.0f);
	case EPlaygroundRarity::Epic:
		return FLinearColor(0.70f, 0.20f, 1.00f, 1.0f);
	case EPlaygroundRarity::Legendary:
		return FLinearColor(1.00f, 0.55f, 0.00f, 1.0f);
	default:
		return FLinearColor::White;
	}
}
