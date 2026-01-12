// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Spatial/Playground_SpatialInventory.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Widgets/Spatial/Playground_InventoryGrid.h"
#include "Inventory/Utils/Playground_InventoryStatics.h"

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


