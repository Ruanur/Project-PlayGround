// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/SlottedItems/Playground_EquippedSlottedItem.h"

FReply UPlayground_EquippedSlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnEquippedSlottedItemClicked.Broadcast(this);
	return FReply::Handled();
}
