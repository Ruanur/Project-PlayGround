// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Playground_FastArray.generated.h"

class UPlayground_InventoryItem;
class UPlayground_InventoryComponent;
class UPlayground_ItemComponent;

//인벤토리 단일 항목
USTRUCT(BlueprintType)
struct FPlayground_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FPlayground_InventoryEntry() {}

private:
	friend struct FPlayground_InventoryFastArray;
	friend UPlayground_InventoryComponent;

	UPROPERTY()
	TObjectPtr<UPlayground_InventoryItem> Item = nullptr;
};


//아이템 리스트
USTRUCT(BlueprintType)
struct FPlayground_InventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

	FPlayground_InventoryFastArray() : OwnerComponent(nullptr) {}
	FPlayground_InventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	TArray<UPlayground_InventoryItem*> PG_GetAllItems() const;
	//FFastArraySerializer contract
	void PG_PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PG_PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);

	//End of FFastArraySerializer contract
	bool NewDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FPlayground_InventoryEntry, FPlayground_InventoryFastArray>(Entries, DeltaParams, *this);
	}

	UPlayground_InventoryItem* AddEntry(UPlayground_ItemComponent* ItemComponent);
	UPlayground_InventoryItem* AddEntry(UPlayground_InventoryItem* Item);
	void RemoveEntry(UPlayground_InventoryItem* Item);


private:
	friend UPlayground_InventoryComponent;

	//Replicated list of Items
	UPROPERTY()
	TArray<FPlayground_InventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;

};

template<>
struct TStructOpsTypeTraits<FPlayground_InventoryFastArray> : public TStructOpsTypeTraitsBase2<FPlayground_InventoryFastArray>
{
	enum { WithNetDeltaSerializer = true };
};





