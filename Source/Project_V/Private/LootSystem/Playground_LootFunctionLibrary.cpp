// Copyright 2025. Jinsol Co. All rights reserved


#include "LootSystem/Playground_LootFunctionLibrary.h"
#include "LootSystem/Playground_LootTable.h"
#include "Items/Drops/PlaygroundDropsBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

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

APlaygroundDropsBase* UPlayground_LootFunctionLibrary::SpawnLootFromTable(UObject* WorldContextObject, const UPlayground_LootTable* LootTable, const FTransform& SpawnTransform, AActor* Owner)
{
	if (!WorldContextObject || !LootTable) return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;
	
	const EPlaygroundRarity ChosenRarity = RollRarity(LootTable);

	Debug::Print(
		FString::Printf(TEXT("SpawnLootFromTable: ChosenRarity = %d"), static_cast<int32>(ChosenRarity)),
		FColor::Cyan
	);

	const FPlayground_LootEntry* ChosenEntry = RollEntry(LootTable, ChosenRarity);

	if (!ChosenEntry || !ChosenEntry->ItemActorClass)
	{
		Debug::Print(TEXT("SpawnLootFromTable: ChosenEntry is null or ItemActorClass is null"), FColor::Red);
		return nullptr;
	}

	Debug::Print(
		FString::Printf(TEXT("SpawnLootFromTable: Selected ItemActorClass = %s"), *ChosenEntry->ItemActorClass->GetName()),
		FColor::Green
	);

	APlaygroundDropsBase* SpawnedItem = World->SpawnActorDeferred<APlaygroundDropsBase>(
		ChosenEntry->ItemActorClass, 
		SpawnTransform, 
		Owner, 
		nullptr, 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!SpawnedItem)
	{
		Debug::Print(TEXT("SpawnLootFromTable: SpawnActorDeferred failed"), FColor::Red);
		return nullptr;
	}

	UGameplayStatics::FinishSpawningActor(SpawnedItem, SpawnTransform);

	Debug::Print(
		FString::Printf(TEXT("SpawnLootFromTable: InitializeDropFromRarity(%d)"), static_cast<int32>(ChosenRarity)),
		FColor::Yellow
	);

	SpawnedItem->InitializeDropFromRarity(ChosenRarity);

	return SpawnedItem;
}

EPlaygroundRarity UPlayground_LootFunctionLibrary::RollRarity(const UPlayground_LootTable* LootTable)
{
	if (!LootTable)
	{
		Debug::Print(TEXT("Roll Rarity : LootTable is Null, fallback Common"), FColor::Red);
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
		Debug::Print(TEXT("RollRarity: TotalWeight <= 0, fallback Common"), FColor::Red);
		return EPlaygroundRarity::Common;
	}

	float OriginalRoll = FMath::FRandRange(0.f, TotalWeight);
	float Roll = OriginalRoll;

	EPlaygroundRarity Result = EPlaygroundRarity::Common;

	Roll -= CommonWeight;
	if (Roll <= 0.f)
	{
		Result = EPlaygroundRarity::Common;
	}
	else
	{
		Roll -= UncommonWeight;
		if (Roll <= 0.f)
		{
			Result = EPlaygroundRarity::Uncommon;
		}
		else
		{
			Roll -= RareWeight;
			if (Roll <= 0.f)
			{
				Result = EPlaygroundRarity::Rare;
			}
			else
			{
				Roll -= EpicWeight;
				if (Roll <= 0.f)
				{
					Result = EPlaygroundRarity::Epic;
				}
				else
				{
					Result = EPlaygroundRarity::Legendary;
				}
			}
		}
	}

	Debug::Print(
		FString::Printf(
			TEXT("RollRarity: Result = %d | Roll = %.2f / TotalWeight = %.2f"),
			static_cast<int32>(Result),
			OriginalRoll,
			TotalWeight
		),
		FColor::Yellow
	);

	return Result;
}

const FPlayground_LootEntry* UPlayground_LootFunctionLibrary::RollEntry(const UPlayground_LootTable* LootTable, EPlaygroundRarity InRarity)
{
	if (!LootTable)
	{
		Debug::Print(TEXT("RollEntry: LootTable is null"), FColor::Red);
		return nullptr;
	}

	const TArray<FPlayground_LootEntry>* Pool = LootTable->GetPool(InRarity);

	if (!Pool || Pool->IsEmpty())
	{
		Debug::Print(
			FString::Printf(TEXT("RollEntry: Pool is null or empty for rarity = %d"), static_cast<int32>(InRarity)),
			FColor::Red
		);
		return nullptr;
	}

	Debug::Print(
		FString::Printf(TEXT("RollEntry: Rarity = %d, Pool Num = %d"),
			static_cast<int32>(InRarity),
			Pool->Num()),
		FColor::Cyan
	);


	return PlaygroundLootInternal::RollWeightEntry(*Pool);
}
