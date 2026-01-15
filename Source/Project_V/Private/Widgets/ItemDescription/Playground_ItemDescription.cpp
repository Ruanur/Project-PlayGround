// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/ItemDescription/Playground_ItemDescription.h"

#include "Components/SizeBox.h"

FVector2D UPlayground_ItemDescription::GetBoxSize() const
{
	return SizeBox->GetDesiredSize();
}