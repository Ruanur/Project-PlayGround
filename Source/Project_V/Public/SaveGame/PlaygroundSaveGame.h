// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "Inventory/Save/Playground_FInventorySlotInfo.h"
#include "PlaygroundSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	EPlaygroundGameDifficulty SavedCurrentGameDifficulty;

	UPROPERTY(BlueprintReadWrite)
	float SavedLookSensitivityYaw = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float SavedLookSensitivityPitch = 1.0f;

	UPROPERTY(SaveGame)
	TArray<FInventorySlotInfo> SavedInventory;
};
