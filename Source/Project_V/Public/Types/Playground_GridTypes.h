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

UENUM(BlueprintType)
enum class EPlayground_TileQuadrant : uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

USTRUCT(BlueprintType)
struct FPlayground_TileParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FIntPoint TileCoordinates{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 TileIndex{ INDEX_NONE };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	EPlayground_TileQuadrant TileQuadrant{ EPlayground_TileQuadrant::None };
};

inline bool operator==(const FPlayground_TileParameters& A, const FPlayground_TileParameters& B)
{
	return A.TileCoordinates == B.TileCoordinates && A.TileIndex == B.TileIndex && A.TileQuadrant == B.TileQuadrant;
}

USTRUCT()
struct FPlayground_SpaceQueryResult
{
	GENERATED_BODY()

	// True if the space queried has no items in it
	bool bHasSpace{ false };

	// Valid if there's a single item we can swap with
	TWeakObjectPtr<UPlayground_InventoryItem> ValidItem = nullptr;

	// Upper left index of the valid item, if there is one
	int32 UpperLeftIndex{ INDEX_NONE };

};