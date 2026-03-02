// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Drops/Manifest/Playground_ItemManifest.h"

#include "Items/Drops/Playground_InventoryItem.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "Items/Fragment/Playground_ItemFragment.h"
#include "Widgets/Composite/Playground_CompositeBase.h"

UPlayground_InventoryItem* FPlayground_ItemManifest::Manifest(UObject* NewOuter)
{
	UPlayground_InventoryItem* Item = NewObject<UPlayground_InventoryItem>(NewOuter, UPlayground_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);
	for (auto& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();
	}

	PG_ClearFragments();

	return Item;
}

void FPlayground_ItemManifest::AssimilateInventoryFragments(UPlayground_CompositeBase* Composite) const
{
	const auto& InventoryItemFragments = GetAllFragmentsOfType<FPlayground_InventoryItemFragment>();
	for (const auto* Fragment : InventoryItemFragments)
	{
		Composite->ApplyFunction([Fragment](UPlayground_CompositeBase* Widget)
			{
				Fragment->Assimilate(Widget);
			});
	}
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

void FPlayground_ItemManifest::PG_ClearFragments()
{
	for (auto& Fragment : Fragments)
	{
		Fragment.Reset();
	}
	Fragments.Empty();
}
