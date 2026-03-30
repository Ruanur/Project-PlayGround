// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "Playground_LootFunctionLibrary.generated.h"

/**
 * 
 */

class UPlayground_LootTable;
class APlayground_WorldDroppedItem;
struct FPlayground_LootEntry;

UCLASS()
class PROJECT_V_API UPlayground_LootFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Loot", meta = (WorldContext = "WorldContextObject"))
	static APlayground_WorldDroppedItem* SpawnLootFromTable(
		UObject* WorldContextObject,
		const UPlayground_LootTable* LootTable,
		const FTransform& SpawnTransform,
		AActor* Owner = nullptr
	);

	UFUNCTION(BlueprintPure, Category = "Loot")
	static EPlaygroundRarity RollRarity(const UPlayground_LootTable* LootTable);

	static const FPlayground_LootEntry* RollEntry(
		const UPlayground_LootTable* LootTable,
		EPlaygroundRarity InRarity
	);
};
