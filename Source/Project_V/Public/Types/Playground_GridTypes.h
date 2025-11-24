// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Playground_GridTypes.generated.h"

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
