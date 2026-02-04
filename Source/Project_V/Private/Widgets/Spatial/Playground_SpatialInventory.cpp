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
#include "Widgets/SlottedItems/Playground_EquippedSlottedItem.h"
#include "Inventory/Playground_InventoryComponent.h"

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

	UPlayground_HoverItem* HoverItem = GetHoverItem();

	// Create an Equipped Slotted Item and add it to Equipped Grid Slot (call EquippedGridSlot->OnItemEquipped()
	const float TileSize = UPlayground_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize();
	UPlayground_EquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(
		HoverItem->GetInventoryItem(),
		EquipmentTypeTag,
		TileSize
	);

	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);

	// Inform the server that we've equipped an item (potentially unequipped an item as well)
	UPlayground_InventoryComponent* InventoryComponent = UPlayground_InventoryStatics::PG_GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));

	InventoryComponent->Server_EquipSlotClicked(HoverItem->GetInventoryItem(), nullptr);

	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(HoverItem->GetInventoryItem());
	}

	// Clear the Hover Item
	Grid_Equippables->PG_ClearHoverItem();
}

void UPlayground_SpatialInventory::EquippedSlottedItemClicked(UPlayground_EquippedSlottedItem* EquippedSlottedItem)
{
	// Remove the Item Description 
	UPlayground_InventoryStatics::ItemUnhovered(GetOwningPlayer());

	if(IsValid(GetHoverItem()) && GetHoverItem()->IsStackable()) return;
	
	// Get Item to Equip
	UPlayground_InventoryItem* ItemToEquip = IsValid(GetHoverItem()) ? GetHoverItem()->GetInventoryItem() : nullptr;

	// Get Item to Unequip
	UPlayground_InventoryItem* ItemToUnequip = EquippedSlottedItem->GetInventoryItem();
		
	// Get the Equipped Grid Slot holding this item
	UPlayground_EquippedGridSlot* EquippedGridSlot = FindSlotWithEquippedItem(ItemToUnequip);

	// Clear the equipped grid slot of this item (set it's inventory item to nullptr)
	ClearSlotOfItem(EquippedGridSlot);

	// Assign previously equipped item as the hover item
	Grid_Equippables->AssignHoverItem(ItemToUnequip);

	// Remove of the equipped slotted item from the eqiupped grid slot (unbind from the OnEquippedSlottedItemClicked)
	RemoveEquippedSlottedItem(EquippedSlottedItem);
	
	// Make a new equipped slotted item (for the item we held in HoverItem)
	MakeEquippedSlottedItem(EquippedSlottedItem, EquippedGridSlot, ItemToEquip);

	// Broadcast delegates for OnItemEquipped/OnItemUnequipped (from the IC)
	BroadcastSlotClickedDelegates(ItemToEquip, ItemToUnequip);
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
			GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
			Manifest.AssimilateInventoryFragments(DescriptionWidget);
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

float UPlayground_SpatialInventory::GetTileSize() const
{
	return Grid_Equippables->GetTileSize();
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

UPlayground_EquippedGridSlot* UPlayground_SpatialInventory::FindSlotWithEquippedItem(UPlayground_InventoryItem* EquippedItem) const
{
	auto* FoundEquippedGridSlot = EquippedGridSlots.FindByPredicate([EquippedItem](const UPlayground_EquippedGridSlot* GridSlot)
		{
			return GridSlot->GetInventoryItem() == EquippedItem;
		});

	return FoundEquippedGridSlot ? *FoundEquippedGridSlot : nullptr;
}

void UPlayground_SpatialInventory::ClearSlotOfItem(UPlayground_EquippedGridSlot* EquippedGridSlot)
{
	if (IsValid(EquippedGridSlot))
	{
		EquippedGridSlot->SetEquippedSlottedItem(nullptr);
		EquippedGridSlot->SetInventoryItem(nullptr);
	}
}

void UPlayground_SpatialInventory::RemoveEquippedSlottedItem(UPlayground_EquippedSlottedItem* EquippedSlottedItem)
{
	if (!IsValid(EquippedSlottedItem)) return;

	if (EquippedSlottedItem->OnEquippedSlottedItemClicked.IsAlreadyBound(this, &ThisClass::EquippedSlottedItemClicked))
	{
		EquippedSlottedItem->OnEquippedSlottedItemClicked.RemoveDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	}
	EquippedSlottedItem->RemoveFromParent();
}

void UPlayground_SpatialInventory::MakeEquippedSlottedItem(UPlayground_EquippedSlottedItem* EquippedSlottedItem, UPlayground_EquippedGridSlot* EquippedGridSlot, UPlayground_InventoryItem* ItemToEquip)
{
	if (!IsValid(EquippedGridSlot)) return;

	UPlayground_EquippedSlottedItem* SlottedItem = EquippedGridSlot->OnItemEquipped(ItemToEquip, 
		EquippedSlottedItem->GetEquipmentTypeTag(), 
		UPlayground_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize());
	if (IsValid(SlottedItem)) SlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);

	EquippedGridSlot->SetEquippedSlottedItem(SlottedItem);
}

void UPlayground_SpatialInventory::BroadcastSlotClickedDelegates(UPlayground_InventoryItem* ItemToEquip, UPlayground_InventoryItem* ItemToUnequip) const
{
	UPlayground_InventoryComponent* InventoryComponent = UPlayground_InventoryStatics::PG_GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));
	InventoryComponent->Server_EquipSlotClicked(ItemToEquip, ItemToUnequip);

	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(ItemToEquip);
		InventoryComponent->OnItemUnequipped.Broadcast(ItemToUnequip);
	}
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
	if (ActiveGrid.IsValid())
	{
		ActiveGrid->PG_HideCursor();
		ActiveGrid->OnHide();
	}

	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) ActiveGrid->PG_ShowCursor();

	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}



