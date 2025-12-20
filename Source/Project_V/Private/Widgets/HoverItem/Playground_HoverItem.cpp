// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/HoverItem/Playground_HoverItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/Drops/Playground_InventoryItem.h"

void UPlayground_HoverItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);

}

void UPlayground_HoverItem::UpdateStackCount(const int32 Count) const
{
	if (Count > 0)
	{
		Text_StackCount->SetText(FText::AsNumber(Count));
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FGameplayTag UPlayground_HoverItem::GetItemType() const
{
	if (InventoryItem.IsValid())
	{
		return InventoryItem->GetItemManifest().GetItemType();
	}

	return FGameplayTag();
}

void UPlayground_HoverItem::SetIsStackable(bool bStacks)
{
	bIsStackable = bStacks;
	if (!bStacks)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UPlayground_InventoryItem* UPlayground_HoverItem::GetInventoryItem() const
{
	return InventoryItem.Get(); 
}

void UPlayground_HoverItem::SetInventoryItem(UPlayground_InventoryItem* Item)
{
	InventoryItem = Item;
}