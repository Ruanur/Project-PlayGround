// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Playground_FInventorySlotInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
    GENERATED_BODY()

    // DataTable / Manifest ID
    UPROPERTY(SaveGame)
    FGameplayTag ItemType;

    // Grid 위치
    UPROPERTY(SaveGame)
    int32 UpperLeftIndex = INDEX_NONE;

    // 스택 수
    UPROPERTY(SaveGame)
    int32 StackAmount = 1;
};
