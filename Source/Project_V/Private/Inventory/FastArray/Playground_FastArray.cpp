// Copyright 2025. Jinsol Co. All rights reserved


#include "Inventory/FastArray/Playground_FastArray.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Items/Drops/Playground_ItemComponent.h"

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
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	// 엔트리 생성 (참조)
	FPlayground_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();


	// Manifest 복사, 원본 깨짐 방지
	FPlayground_ItemManifest ManifestCopy = ItemComponent->GetItemManifest();
	NewEntry.Item = ManifestCopy.Manifest(OwningActor, ItemComponent->GetItemRarity());

	if (UPlayground_InventoryComponent* IC = Cast<UPlayground_InventoryComponent>(OwnerComponent))
	{
		IC->AddRepSubObject(NewEntry.Item);
	}

	//FPlayground_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	//NewEntry.Item = ItemComponent->GetItemManifest().Manifest(OwningActor);

	//IC->AddRepSubObject(NewEntry.Item);

	MarkItemDirty(NewEntry);
	return NewEntry.Item;
}

UPlayground_InventoryItem* FPlayground_InventoryFastArray::AddEntry(UPlayground_InventoryItem* Item)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	// 참조로 받아야 Entries에 저장됨
	FPlayground_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	// 로드 아이템도 등록
	if (UPlayground_InventoryComponent* IC = Cast<UPlayground_InventoryComponent>(OwnerComponent))
	{
		IC->AddRepSubObject(NewEntry.Item);
	}

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

UPlayground_InventoryItem* FPlayground_InventoryFastArray::FindFirstItemByType(const FGameplayTag& ItemType)
{
	auto* FoundItem = Entries.FindByPredicate([ItemType = ItemType](const FPlayground_InventoryEntry& Entry)
		{
			return IsValid(Entry.Item) && Entry.Item->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
		});

	return FoundItem ? FoundItem->Item : nullptr;
}