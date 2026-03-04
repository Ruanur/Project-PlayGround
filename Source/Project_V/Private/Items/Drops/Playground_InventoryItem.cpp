// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Drops/Playground_InventoryItem.h"
#include "Net/UnrealNetwork.h"
#include "Items/Fragment/Playground_ItemFragment.h"
#include "Items/Fragment/Playground_FragmentTags.h"

#include "PlaygroundDebugHelper.h"
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

FText UPlayground_InventoryItem::PG_GetDisplayName() const
{
	FText DisplayName = FText::FromName(GetItemID());

	const FPlayground_ItemManifest& Manifest = GetItemManifest();

	const FGameplayTag NameTag = FragmentTags::ItemNameFragment.GetTag();

	if (const FPlayground_TextFragment* NameFrag = Manifest.GetFragmentOfTypeWithTag<FPlayground_TextFragment>(NameTag))
	{
		const FText NameText = NameFrag->GetText();
		if (!NameText.IsEmpty())
		{
			return NameText;
		}
	}

	return DisplayName;
}
