// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Playground_FInventorySlotInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
    GENERATED_BODY()

    UPROPERTY()
    TSoftClassPtr<UPlayground_InventoryItem> ItemClass;

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
    explicit FInventorySlotInfo(TSubclassOf<UPlayground_InventoryItem> InClass, int32 Index, int32 UpperLeftIndex, bool bInIsStackable, int32 InStack = 0)
        : ItemClass(InClass)
        , Index(Index)
        , UpperLeftIndex(UpperLeftIndex)
        , bIsStackable(bInIsStackable)
        , StackAmount(InStack)
    {
    }

};
