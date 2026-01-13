// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Drops/Playground_InventoryItem.h"
#include "Net/UnrealNetwork.h"
#include "Items/Fragment/Playground_ItemFragment.h"

void UPlayground_InventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ThisClass, ItemManifest);
	DOREPLIFETIME(ThisClass, TotalStackCount);
}

void UPlayground_InventoryItem::SetItemManifest(const FPlayground_ItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FPlayground_ItemManifest>(Manifest);
}

bool UPlayground_InventoryItem::IsStackable() const
{
	const FPlayground_StackableFragment* Stackable = GetItemManifest().GetFragmentOfType<FPlayground_StackableFragment>();
	return Stackable != nullptr;
}

bool UPlayground_InventoryItem::IsConsumable() const
{
	return GetItemManifest().GetItemCategory() == EPlayground_ItemCategory::Consumable;
}