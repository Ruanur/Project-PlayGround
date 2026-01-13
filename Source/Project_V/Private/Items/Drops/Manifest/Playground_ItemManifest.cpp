// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Drops/Manifest/Playground_ItemManifest.h"

#include "Items/Drops/Playground_InventoryItem.h"
#include "Items/Drops/Playground_ItemComponent.h"

UPlayground_InventoryItem* FPlayground_ItemManifest::Manifest(UObject* NewOuter)
{
	UPlayground_InventoryItem* Item = NewObject<UPlayground_InventoryItem>(NewOuter, UPlayground_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	return Item;
}

void FPlayground_ItemManifest::PG_SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!IsValid(PickupActorClass) || !IsValid(WorldContextObject)) return;

	AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation);
	if (!IsValid(SpawnedActor)) return;

	// Set the item manifest, item category, item type, etc
	UPlayground_ItemComponent* ItemComp = SpawnedActor->FindComponentByClass<UPlayground_ItemComponent>();
	check(ItemComp);

	ItemComp->InitItemManifest(*this);
}
