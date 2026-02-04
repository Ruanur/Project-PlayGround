// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/GridSlots/Playground_EquippedGridSlot.h"
#include "Widgets/HoverItem/Playground_HoverItem.h"
#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Components/Image.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Items/Fragment/Playground_FragmentTags.h"
#include "Items/Fragment/Playground_ItemFragment.h"
#include "Components/Overlay.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Widgets/SlottedItems/Playground_EquippedSlottedItem.h"
#include "Components/OverlaySlot.h"

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

	if (IsValid(EquippedSlottedItem)) return;

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
	const FPlayground_GridFragment* GridFragment = GetFragment<FPlayground_GridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return nullptr;
	const FIntPoint GridDimensions = GridFragment->GetGridSize();

	// Calculate the Draw Size for the Equipped Slotted Item
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	const FVector2D DrawSize = GridDimensions * IconTileWidth;

	// Create the Equipped Slotted Item widget
	EquippedSlottedItem = CreateWidget<UPlayground_EquippedSlottedItem>(GetOwningPlayer(), EquippedSlottedItemClass);

	// Set the Slotted Item's Inventory Item
	EquippedSlottedItem->SetInventoryItem(Item);

	// Set the Slotted Item's Equipment Type Tag
	EquippedSlottedItem->SetEquipmentTypeTag(EquipmentTag);

	// Hide the Stack Count widget on the Slotted Item
	EquippedSlottedItem->PG_UpdateStackCount(0);

	// Set Inventory Item on this class (The Equipped Grid Slot)
	SetInventoryItem(Item);

	// Set the Image Brush on the Equipped Slotted item
	const FPlayground_ImageFragment* ImageFragment = GetFragment<FPlayground_ImageFragment>(Item, FragmentTags::IconFragment);
	if (!ImageFragment) return nullptr;

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = DrawSize;

	EquippedSlottedItem->SetImageBrush(Brush);

	// Add the Slotted Item as a child to this widget's Overlay
	Overlay_Root->AddChildToOverlay(EquippedSlottedItem);
	FGeometry OverlayGeometry = Overlay_Root->GetCachedGeometry();
	auto OverlayPos = OverlayGeometry.Position;
	auto OverlaySize = OverlayGeometry.Size;

	const float LeftPadding = OverlaySize.X / 2.f - DrawSize.X / 2.f;
	const float TopPadding = OverlaySize.Y / 2.f - DrawSize.Y / 2.f;

	UOverlaySlot* OverlaySlot = UWidgetLayoutLibrary::SlotAsOverlaySlot(EquippedSlottedItem);
	OverlaySlot->SetPadding(FMargin(LeftPadding, TopPadding));

	// Return the Equipped Slotted item widget
	return EquippedSlottedItem;
}
