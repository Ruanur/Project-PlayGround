// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "Inventory/Save/Playground_FInventorySlotInfo.h"
#include "PlayergroundTypes/PlaygroundStructTypes.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float MasterVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float BGMVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float SFXVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float UIVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	FString SlotName = TEXT("AudioSettings");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	int32 UserIndex = 0;

	UPROPERTY(SaveGame)
	TArray<FInventorySlotInfo> SavedEquippable;

	UPROPERTY(SaveGame)
	TArray<FInventorySlotInfo> SavedConsumable;

	UPROPERTY(SaveGame)
	TArray<FInventorySlotInfo> SavedCraftable;

	UPROPERTY(SaveGame)
	TArray<FEquippedSlotInfo> SavedEquipmentSlots;

	UPROPERTY(SaveGame)
	TArray<FPlayground_QuickSlotRef> SavedQuickSlots;
};
