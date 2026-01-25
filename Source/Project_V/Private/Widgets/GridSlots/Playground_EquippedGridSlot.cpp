// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/GridSlots/Playground_EquippedGridSlot.h"
#include "Widgets/HoverItem/Playground_HoverItem.h"
#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Components/Image.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Items/Fragment/Playground_FragmentTags.h"
#include "Items/Fragment/Playground_ItemFragment.h"

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

UPlayground_EquippedSlottedItem* UPlayground_EquippedGridSlot::OnItemEquipped(UPlayground_InventoryItem* Item, const FGameplayTag& EquipmentTag, float TileSize)
{
	// Check the Equipment Type Tag
	if (!EquipmentTag.MatchesTagExact(EquipmentTag)) return nullptr;
	 
	// Get Grid Dimensions
	
	
	// Calculate the Draw Size for the Equipped Slotted Item
	const FPlayground_GridFragment* GridFragment = GetFragment<FPlayground_GridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return nullptr;

	const FIntPoint GridDimensions = GridFragment->GetGridSize();

	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	const FVector2D DrawSize = GridDimensions * IconTileWidth;

	// Create the Equipped Slotted Item widget
	
	// Set the Slotted Item's Inventory Item
	
	// Set the Slotted Item's Equipment Type Tag
	
	// Hide the Stack Count widget on the Slotted Item
	
	// Set Inventory Item on this class (The Equipped Grid Slot)
	
	// Set the Image Brush on the Equipped Slotted item
	
	// Add the Slotted Item as a child to this widget's Overlay
	
	// Return the Equipped Slotted item widget

	return nullptr;
}
