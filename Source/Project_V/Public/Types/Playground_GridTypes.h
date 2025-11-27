// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Playground_GridTypes.generated.h"


class UPlayground_InventoryItem;
/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayground_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	None
};

USTRUCT()
struct FPlayground_SlotAvailability
{
	GENERATED_BODY()

	FPlayground_SlotAvailability() {}
	FPlayground_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}

	int32 Index{ INDEX_NONE };
	int32 AmountToFill{ 0 };
	bool bItemAtIndex{ false };

};

USTRUCT()
struct FPlayground_SlotAvailabilityResult
{
	GENERATED_BODY()

	FPlayground_SlotAvailabilityResult() {}

	TWeakObjectPtr<UPlayground_InventoryItem> Item;
	int32 TotalRoomToFill{ 0 };
	int32 Remainder{ 0 };
	bool bStackable{ false };
	TArray<FPlayground_SlotAvailability> SlotAvailabilities;
};
