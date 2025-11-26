// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Utils/Playground_WidgetUtils.h"

int32 UPlayground_WidgetUtils::PG_GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}
