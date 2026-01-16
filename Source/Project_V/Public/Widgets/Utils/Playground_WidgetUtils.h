// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Playground_WidgetUtils.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_WidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FVector2D PG_GetWidgetPosition(UWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FVector2D PG_GetWidgetSize(UWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool PG_IsWithinBounds(const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos);

	static FVector2D GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, const FVector2D& MousePos);

	static int32 PG_GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);
	static FIntPoint PG_GetPositionFromIndex(const int32 Index, const int32 Columns);
};
