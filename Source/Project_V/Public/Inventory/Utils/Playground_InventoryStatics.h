// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Playground_InventoryStatics.generated.h"

/**
 * 
 */
class UPlayground_InventoryComponent;
class UPlayground_ItemComponent;

UCLASS()
class PROJECT_V_API UPlayground_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory") 
	static UPlayground_InventoryComponent* PG_GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EPlayground_ItemCategory GetItemCategoryFromItemComp(UPlayground_ItemComponent* ItemComp);
	
};
