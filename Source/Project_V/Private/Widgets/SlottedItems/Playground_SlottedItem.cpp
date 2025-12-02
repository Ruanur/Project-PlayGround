// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/SlottedItems/Playground_SlottedItem.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Components/Image.h"

void UPlayground_SlottedItem::SetInventoryItem(UPlayground_InventoryItem* Item)
{
	InventoryItem = Item;
}

void UPlayground_SlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}
