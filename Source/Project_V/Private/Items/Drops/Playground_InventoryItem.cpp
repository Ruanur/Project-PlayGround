// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Drops/Playground_InventoryItem.h"
#include "Net/UnrealNetwork.h"

void UPlayground_InventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UPlayground_InventoryItem::SetItemManifest(const FPlayground_ItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FPlayground_ItemManifest>(Manifest);
}
