// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Drops/Manifest/Playground_ItemManifest.h"

#include "Items/Drops/Playground_InventoryItem.h"

UPlayground_InventoryItem* FPlayground_ItemManifest::Manifest(UObject* NewOuter)
{
	UPlayground_InventoryItem* Item = NewObject<UPlayground_InventoryItem>(NewOuter, UPlayground_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	return Item;
}