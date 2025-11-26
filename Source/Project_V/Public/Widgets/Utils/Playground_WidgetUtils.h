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
	static int32 PG_GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);
	
};
