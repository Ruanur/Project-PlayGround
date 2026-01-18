// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Composite/Playground_CompositeBase.h"

void UPlayground_CompositeBase::PG_Collapse()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayground_CompositeBase::PG_Expand()
{
	SetVisibility(ESlateVisibility::Visible);
}
