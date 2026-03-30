// Copyright 2025. Jinsol Co. All rights reserved


#include "LootSystem/Playground_WorldDroppedItem.h"
#include "Items/Drops/Playground_InventoryItem.h"

void APlayground_WorldDroppedItem::InitializedDroppedItem(const FPlayground_ItemManifest& InManifest, EPlaygroundRarity InRarity)
{
	DroppedRarity = InRarity;

	FPlayground_ItemManifest ManifestCopy = InManifest;
	InventoryItem = ManifestCopy.Manifest(this, InRarity);
}
