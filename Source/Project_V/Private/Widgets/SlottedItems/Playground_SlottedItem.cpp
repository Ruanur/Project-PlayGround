// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/SlottedItems/Playground_SlottedItem.h"
#include "Items/Drops/Playground_InventoryItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Utils/Playground_InventoryStatics.h"

FReply UPlayground_SlottedItem::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnSlottedItemClicked.Broadcast(GridIndex, MouseEvent);
	return FReply::Handled();
}

void UPlayground_SlottedItem::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UPlayground_InventoryStatics::ItemHovered(GetOwningPlayer(), InventoryItem.Get());
}

void UPlayground_SlottedItem::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	UPlayground_InventoryStatics::ItemUnhovered(GetOwningPlayer());
}

void UPlayground_SlottedItem::SetInventoryItem(UPlayground_InventoryItem* Item)
{
	InventoryItem = Item;
}

void UPlayground_SlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UPlayground_SlottedItem::PG_UpdateStackCount(int32 StackCount)
{
	if (StackCount > 0)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
		Text_StackCount->SetText(FText::AsNumber(StackCount));
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}
