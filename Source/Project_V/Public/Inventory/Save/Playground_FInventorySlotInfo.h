// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Playground_FInventorySlotInfo.generated.h"

/**
 * 
 */

// SaveGame 직렬화 구조체 (Inventory)
USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    FName ItemID;
    //TObjectPtr<UPlayground_InventoryItem> Item;
    
    UPROPERTY(SaveGame)
    FGuid InstanceID;

    UPROPERTY(SaveGame)
    int32 Index = INDEX_NONE;

    UPROPERTY(SaveGame)
    int32 UpperLeftIndex = INDEX_NONE;

    UPROPERTY(SaveGame)
    bool bIsStackable = false;

    UPROPERTY(SaveGame)
    int32 StackAmount = 0;

    // Default constructor
    FInventorySlotInfo() = default;

    // Convenience Constructor - matches what I use in Emplace 
    explicit FInventorySlotInfo(FName InItemID, FGuid InInstanceID, int32 Index, int32 UpperLeftIndex, bool bInIsStackable, int32 InStack = 0)
        : ItemID(InItemID)
        , InstanceID(InInstanceID)
        , Index(Index)
        , UpperLeftIndex(UpperLeftIndex)
        , bIsStackable(bInIsStackable)
        , StackAmount(InStack)
    {
    }

};
