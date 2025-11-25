// Copyright 2025. Jinsol Co. All rights reserved


#include "Inventory/FastArray/Playground_FastArray.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Inventory/Playground_InventoryComponent.h"

TArray<UPlayground_InventoryItem*> FPlayground_InventoryFastArray::PG_GetAllItems() const
{
	TArray<UPlayground_InventoryItem*> Results;
	Results.Reserve(Entries.Num());
	for (const auto& Entry : Entries)
	{
		if (!IsValid(Entry.Item)) continue;
		Results.Add(Entry.Item);
	}
	return Results;

}

void FPlayground_InventoryFastArray::PG_PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	UPlayground_InventoryComponent* IC = Cast<UPlayground_InventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return;

	for (int32 Index : RemovedIndices)
	{
		IC->OnItemRemoved.Broadcast(Entries[Index].Item);
	}
}

void FPlayground_InventoryFastArray::PG_PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UPlayground_InventoryComponent* IC = Cast<UPlayground_InventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return;

	for (int32 Index : AddedIndices)
	{
		IC->OnItemAdded.Broadcast(Entries[Index].Item);
	}
}

UPlayground_InventoryItem* FPlayground_InventoryFastArray::AddEntry(UPlayground_ItemComponent* ItemComponent)
{
	// TODO : Implement once ItemComponent is more complete
	return nullptr;
}

UPlayground_InventoryItem* FPlayground_InventoryFastArray::AddEntry(UPlayground_InventoryItem* Item)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FPlayground_InventoryEntry NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	MarkItemDirty(NewEntry);
	return Item;
}

void FPlayground_InventoryFastArray::RemoveEntry(UPlayground_InventoryItem* Item)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPlayground_InventoryEntry& Entry = *EntryIt;
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
