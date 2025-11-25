// Copyright 2025. Jinsol Co. All rights reserved


#include "Inventory/FastArray/Playground_FastArray.h"


TArray<UPlayground_InventoryItem*> FPlayground_InventoryFastArray::PG_GetAllItems() const
{
	return TArray<UPlayground_InventoryItem*>();
}

void FPlayground_InventoryFastArray::PG_PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{

}

void FPlayground_InventoryFastArray::PG_PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

UPlayground_InventoryItem* FPlayground_InventoryFastArray::AddEntry(UPlayground_ItemComponent* ItemComponent)
{
	return nullptr;
}

UPlayground_InventoryItem* FPlayground_InventoryFastArray::AddEntry(UPlayground_InventoryItem* Item)
{
	return nullptr;
}

void FPlayground_InventoryFastArray::RemoveEntry(UPlayground_InventoryItem* Item)
{
}
