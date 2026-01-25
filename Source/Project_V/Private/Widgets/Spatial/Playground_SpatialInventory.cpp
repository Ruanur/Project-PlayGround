// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Spatial/Playground_SpatialInventory.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Widgets/Spatial/Playground_InventoryGrid.h"
#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Components/CanvasPanel.h"
#include "Widgets/ItemDescription/Playground_ItemDescription.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/HoverItem/Playground_HoverItem.h"

#include "PlaygroundDebugHelper.h"



void UPlayground_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::PG_ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::PG_ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::PG_ShowCraftables);

	Grid_Equippables->PG_SetOwningCanvas(CanvasPanel);
	Grid_Consumables->PG_SetOwningCanvas(CanvasPanel);
	Grid_Craftables->PG_SetOwningCanvas(CanvasPanel);

	PG_ShowEquippables();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
		{
			UPlayground_EquippedGridSlot* EquippedGridSlot = Cast<UPlayground_EquippedGridSlot>(Widget);
			if (IsValid(EquippedGridSlot))
			{
				EquippedGridSlots.Add(EquippedGridSlot);
				EquippedGridSlot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::EquippedGridSlotClicked);
			}
		});
}

void UPlayground_SpatialInventory::EquippedGridSlotClicked(UPlayground_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag)
{
	// Check to see if we can equip the Hover Item
	if (!CanEquipHoverItem(EquippedGridSlot, EquipmentTypeTag)) return;

	// Create an Equipped Slotted Item and add it to Equipped Grid Slot
	

	// Clear the Hover Item
	// Inform the server that we've equipped an item (potentially unequipped an item as well)
}

FPlayground_SlotAvailabilityResult UPlayground_SpatialInventory::HasRoomForItem(UPlayground_ItemComponent* ItemComponent) const
{
	switch (UPlayground_InventoryStatics::GetItemCategoryFromItemComp(ItemComponent))
	{
	case EPlayground_ItemCategory::Equippable:
		return Grid_Equippables->HasRoomForItem(ItemComponent);
	case EPlayground_ItemCategory::Consumable:
		return Grid_Consumables->HasRoomForItem(ItemComponent);
	case EPlayground_ItemCategory::Craftable:
		return Grid_Craftables->HasRoomForItem(ItemComponent);
	default:
		Debug::Print(TEXT("Invalid Items, ItemComponent doesn't have"));
		return FPlayground_SlotAvailabilityResult();
	}

}

void UPlayground_SpatialInventory::OnItemHovered(UPlayground_InventoryItem* Item)
{
	const auto& Manifest = Item->GetItemManifest();
	UPlayground_ItemDescription* DescriptionWidget = GetItemDescription();
	DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);

	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this, &Manifest, DescriptionWidget]()
		{
			// Assimilate the manifest into the Item Description widget.
			Manifest.AssimilateInventoryFragments(DescriptionWidget);
			GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
		});

	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer, DescriptionTimerDelegate, DescriptionTimerDelay, false);
}

void UPlayground_SpatialInventory::OnItemUnHovered()
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

bool UPlayground_SpatialInventory::HasHoverItem() const
{
	if (Grid_Equippables->PG_HasHoverItem()) return true;
	if (Grid_Consumables->PG_HasHoverItem()) return true;
	if (Grid_Craftables->PG_HasHoverItem()) return true;
	return false;
}

UPlayground_HoverItem* UPlayground_SpatialInventory::GetHoverItem() const
{
	if (!ActiveGrid.IsValid()) return nullptr;
	return ActiveGrid->GetHoverItem();
}

FReply UPlayground_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	ActiveGrid->PG_DropItem();
	return FReply::Handled();
}

void UPlayground_SpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(ItemDescription)) return; 
	SetItemDescriptionSizeAndPosition(ItemDescription, CanvasPanel);
}


void UPlayground_SpatialInventory::SetItemDescriptionSizeAndPosition(UPlayground_ItemDescription* Description, UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	if (!IsValid(ItemDescriptionCPS)) return;
	
	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);

	FVector2D ClampedPosition = UPlayground_WidgetUtils::GetClampedWidgetPosition(
		UPlayground_WidgetUtils::PG_GetWidgetSize(Canvas),
		ItemDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));

	ItemDescriptionCPS->SetPosition(ClampedPosition);
}

bool UPlayground_SpatialInventory::CanEquipHoverItem(UPlayground_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag) const
{
	if (!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid()) return false;

	UPlayground_HoverItem* HoverItem = GetHoverItem();
	if (!IsValid(HoverItem)) return false;

	UPlayground_InventoryItem* HeldItem = HoverItem->GetInventoryItem();

	return HasHoverItem() && IsValid(HeldItem) &&
		!HoverItem->IsStackable() &&
		HeldItem->GetItemManifest().GetItemCategory() == EPlayground_ItemCategory::Equippable &&
		HeldItem->GetItemManifest().GetItemType().MatchesTag(EquipmentTypeTag);
}


UPlayground_ItemDescription* UPlayground_SpatialInventory::GetItemDescription()
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<UPlayground_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescription);
	}

	return ItemDescription;
}

void UPlayground_SpatialInventory::PG_ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UPlayground_SpatialInventory::PG_ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UPlayground_SpatialInventory::PG_ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}



void UPlayground_SpatialInventory::DisableButton(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

void UPlayground_SpatialInventory::SetActiveGrid(UPlayground_InventoryGrid* Grid, UButton* Button)
{
	if (ActiveGrid.IsValid()) ActiveGrid->PG_HideCursor();

	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) ActiveGrid->PG_ShowCursor();

	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}



