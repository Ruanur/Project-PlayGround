// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Utils/Playground_WidgetUtils.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Widget.h"


FVector2D UPlayground_WidgetUtils::PG_GetWidgetPosition(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;

	USlateBlueprintLibrary::LocalToViewport(Widget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry), PixelPosition, ViewportPosition);
	return ViewportPosition; 


}

int32 UPlayground_WidgetUtils::PG_GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint UPlayground_WidgetUtils::PG_GetPositionFromIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}
