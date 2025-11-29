// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Types/Playground_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"

#include "Playground_ItemManifest.generated.h"
/**
 * 
 */
class UPlayground_InventoryItem;

USTRUCT()
struct PROJECT_V_API FPlayground_ItemManifest
{
	GENERATED_BODY()

	UPlayground_InventoryItem* Manifest(UObject* NewOuter);
	EPlayground_ItemCategory GetItemCategory() const { return ItemCategory; }
	FGameplayTag GetItemType() const { return ItemType; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EPlayground_ItemCategory ItemCategory{ EPlayground_ItemCategory::None };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag ItemType;
};