// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/GridSlots/Playground_EquippedGridSlot.h"
#include "Widgets/HoverItem/Playground_HoverItem.h"
#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Components/Image.h"

void UPlayground_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;
	UPlayground_HoverItem* HoverItem = UPlayground_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return; 

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		PG_SetOccupiedTexture();
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayground_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;
	UPlayground_HoverItem* HoverItem = UPlayground_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		PG_SetUnoccupiedTexture();
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
	}
}
 
FReply UPlayground_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag);
	return FReply::Handled();
}
