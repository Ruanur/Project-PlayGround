// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "Items/Drops/Manifest/Playground_ItemManifest.h"
#include "Playground_LootTable.generated.h"

/**
 * 
 */

class APlaygroundDropsBase;

USTRUCT()
struct FPlayground_RarityWeights
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = "0.0"))
	float Common = 60.f;

	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = "0.0"))
	float Uncommon = 25.f;

	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = "0.0"))
	float Rare = 10.f;

	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = "0.0"))
	float Epic = 4.f;

	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = "0.0"))
	float Legendary = 1.f;
};

USTRUCT()
struct FPlayground_LootEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Loot")
	TSubclassOf<APlaygroundDropsBase> ItemActorClass;

	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = "0.0"))
	float Weight = 1.f;
};

UCLASS(BlueprintType)
class PROJECT_V_API UPlayground_LootTable : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Loot")
	FPlayground_RarityWeights RarityWeights;

	UPROPERTY(EditAnywhere, Category = "Loot")
	TArray<FPlayground_LootEntry> CommonPool;
	
	UPROPERTY(EditAnywhere, Category = "Loot")
	TArray<FPlayground_LootEntry> UncommonPool;

	UPROPERTY(EditAnywhere, Category = "Loot")
	TArray<FPlayground_LootEntry> RarePool;

	UPROPERTY(EditAnywhere, Category = "Loot")
	TArray<FPlayground_LootEntry> EpicPool;

	UPROPERTY(EditAnywhere, Category = "Loot")
	TArray<FPlayground_LootEntry> LegendaryPool;

	const TArray<FPlayground_LootEntry>* GetPool(EPlaygroundRarity InRarity) const
	{
		switch (InRarity)
		{
		case EPlaygroundRarity::Common:
			return &CommonPool;
		case EPlaygroundRarity::Uncommon:
			return &UncommonPool;
		case EPlaygroundRarity::Rare:
			return &RarePool;
		case EPlaygroundRarity::Epic:
			return &EpicPool;
		case EPlaygroundRarity::Legendary:
			return &LegendaryPool;
		default:
			return nullptr;
		}
	}
};
