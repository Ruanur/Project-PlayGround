// Copyright 2025. Jinsol Co. All rights reserved


#include "LootSystem/Playground_LootFunctionLibrary.h"
#include "LootSystem/Playground_LootTable.h"
#include "LootSystem/Playground_WorldDroppedItem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

namespace PlaygroundLootInternal
{
	static float GetTotalWeight(const TArray<FPlayground_LootEntry>& Entries)
	{
		float Total = 0.f;
		
		for (const FPlayground_LootEntry& Entry : Entries)
		{
			Total += FMath::Max(0.f, Entry.Weight);
		}
		return Total;
	}

	static const FPlayground_LootEntry* RollWeightEntry(const TArray<FPlayground_LootEntry>& Entries)
	{
		const float TotalWeight = GetTotalWeight(Entries);
		if (TotalWeight <= 0.f) return nullptr;

		float Roll = FMath::FRandRange(0.f, TotalWeight);

		for (const FPlayground_LootEntry& Entry : Entries)
		{
			const float SafeWeight = FMath::Max(0.f, Entry.Weight);
			Roll -= SafeWeight;

			if (Roll <= 0.f)
			{
				return &Entry;
			}
		}
		return Entries.Num() > 0 ? &Entries.Last() : nullptr;
	}
}

APlayground_WorldDroppedItem* UPlayground_LootFunctionLibrary::SpawnLootFromTable(UObject* WorldContextObject, const UPlayground_LootTable* LootTable, const FTransform& SpawnTransform, AActor* Owner)
{
	if (!WorldContextObject || !LootTable) return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;

	const EPlaygroundRarity ChosenRarity = RollRarity(LootTable);
	const FPlayground_LootEntry* ChosenEntry = RollEntry(LootTable, ChosenRarity);

	if (!ChosenEntry || !ChosenEntry->DroppedActorClass)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APlayground_WorldDroppedItem* DroppedActor =
		World->SpawnActor<APlayground_WorldDroppedItem>(
			ChosenEntry->DroppedActorClass,
			SpawnTransform,
			SpawnParams
		);

	if (!DroppedActor)
	{
		return nullptr;
	}

	DroppedActor->InitializedDroppedItem(ChosenEntry->ItemManifest, ChosenRarity);
	return nullptr;
}

EPlaygroundRarity UPlayground_LootFunctionLibrary::RollRarity(const UPlayground_LootTable* LootTable)
{
	if (!LootTable)
	{
		return EPlaygroundRarity::Common;
	}

	const float CommonWeight = FMath::Max(0.f, LootTable->RarityWeights.Common);
	const float UncommonWeight = FMath::Max(0.f, LootTable->RarityWeights.Uncommon);
	const float RareWeight = FMath::Max(0.f, LootTable->RarityWeights.Rare);
	const float EpicWeight = FMath::Max(0.f, LootTable->RarityWeights.Epic);
	const float LegendaryWeight = FMath::Max(0.f, LootTable->RarityWeights.Legendary);

	const float TotalWeight =
		CommonWeight +
		UncommonWeight +
		RareWeight +
		EpicWeight +
		LegendaryWeight;

	if (TotalWeight <= 0.f)
	{
		return EPlaygroundRarity::Common;
	}

	float Roll = FMath::FRandRange(0.f, TotalWeight);

	Roll -= CommonWeight;
	if (Roll <= 0.f) return EPlaygroundRarity::Common;

	Roll -= UncommonWeight;
	if (Roll <= 0.f) return EPlaygroundRarity::Uncommon;

	Roll -= RareWeight;
	if (Roll <= 0.f) return EPlaygroundRarity::Rare;

	Roll -= EpicWeight;
	if (Roll <= 0.f) return EPlaygroundRarity::Epic;

	return EPlaygroundRarity::Legendary;
}

const FPlayground_LootEntry* UPlayground_LootFunctionLibrary::RollEntry(const UPlayground_LootTable* LootTable, EPlaygroundRarity InRarity)
{
	if (!LootTable) return nullptr;

	const TArray<FPlayground_LootEntry>* Pool = LootTable->GetPool(InRarity);
	if (!Pool || Pool->IsEmpty()) return nullptr;

	return PlaygroundLootInternal::RollWeightEntry(*Pool);
}
