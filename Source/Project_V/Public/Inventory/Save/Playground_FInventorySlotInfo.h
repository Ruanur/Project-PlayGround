// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Playground_FInventorySlotInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<UPlayground_InventoryItem> Item = nullptr;

    UPROPERTY()
    int32 Index = INDEX_NONE;

    UPROPERTY()
    int32 UpperLeftIndex = INDEX_NONE;

    UPROPERTY()
    bool bIsStackable = false;

    UPROPERTY()
    int32 StackAmount = 0;

    // Default constructor
    FInventorySlotInfo() = default;

    // Convenience Constructor - matches what I use in Emplace 
    explicit FInventorySlotInfo(UPlayground_InventoryItem* InItem, int32 Index, int32 UpperLeftIndex, int32 InStack = 0)
        : Item(InItem)
        , Index(Index)
        , UpperLeftIndex(UpperLeftIndex)
        , bIsStackable(InItem ? InItem->IsStackable() : false)
        , StackAmount(InStack)
    {
    }
};
