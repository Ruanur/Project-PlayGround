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
		return FLinearColor(0.6196f, 0.6196f, 0.6196f, 1.0f);
	case EPlaygroundRarity::Uncommon:
		return FLinearColor(0.2980f, 0.6863f, 0.3137f, 1.0f);
	case EPlaygroundRarity::Rare:
		return FLinearColor(0.1294f, 0.5882f, 0.9529f, 1.0f);
	case EPlaygroundRarity::Epic:
		return FLinearColor(0.6118f, 0.1529f, 0.6902f, 1.0f);
	case EPlaygroundRarity::Legendary:
		return FLinearColor(1.00f, 0.7020f, 0.2784f, 1.0f);
	default:
		return FLinearColor::White;
	}
}
