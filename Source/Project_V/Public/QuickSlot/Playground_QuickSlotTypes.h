// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "Playground_QuickSlotTypes.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayground_QuickSlotRef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FGuid InstanceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName ItemID;
};
