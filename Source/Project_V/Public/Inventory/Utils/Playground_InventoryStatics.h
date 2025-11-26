// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Playground_InventoryStatics.generated.h"

/**
 * 
 */
class UPlayground_InventoryComponent;

UCLASS()
class PROJECT_V_API UPlayground_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory") 
	static UPlayground_InventoryComponent* PG_GetInventoryComponent(const APlayerController* PlayerController);
};
