// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Spatial/Playground_InventoryGrid.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/GridSlots/Playground_GridSlot.h"
#include "Widgets/Utils/Playground_WidgetUtils.h"
#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "Items/Drops/Manifest/Playground_ItemManifest.h"
#include "Items/Fragment/Playground_ItemFragment.h"
#include "Items/Fragment/Playground_FragmentTags.h"

#include "PlaygroundDebugHelper.h"


void UPlayground_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Debug::Print(TEXT("UPlayground_InventoryGrid::NativeOnInitialized Called"));

	ConstructGrid();

	InventoryComponent = UPlayground_InventoryStatics::PG_GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
}

FPlayground_SlotAvailabilityResult UPlayground_InventoryGrid::HasRoomForItem(const UPlayground_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FPlayground_SlotAvailabilityResult UPlayground_InventoryGrid::HasRoomForItem(const UPlayground_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FPlayground_SlotAvailabilityResult UPlayground_InventoryGrid::HasRoomForItem(const FPlayground_ItemManifest& Manifest)
{
	FPlayground_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;

	FPlayground_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 1;
	SlotAvailability.Index = 0;

	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));

	return Result;
}

void UPlayground_InventoryGrid::AddItem(UPlayground_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;
	
	FPlayground_SlotAvailabilityResult Result = HasRoomForItem(Item);
	AddItemToIndices(Result, Item); 
	//Debug::Print(TEXT("InventoryGrid::AddItemComplated"));

	
}

void UPlayground_InventoryGrid::AddItemToIndices(const FPlayground_SlotAvailabilityResult& Result, UPlayground_InventoryItem* NewItem)
{
	const FPlayground_GridFragment* GridFragment = GetFragment<FPlayground_GridFragment>(NewItem, FragmentTags::GridFragment);
	const FPlayground_ImageFragment* ImageFragment = GetFragment<FPlayground_ImageFragment>(NewItem, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return;
	

	// Get Image Fragment so ew have the icon do display
	// 
	// Create a widget to add to the grid
	// Store the new widget in a container
}

void UPlayground_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);

	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Columns; ++i)
		{
			UPlayground_GridSlot* GridSlot = CreateWidget<UPlayground_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition(i, j);
			GridSlot->SetTileIndex(UPlayground_WidgetUtils::PG_GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}


bool UPlayground_InventoryGrid::MatchesCategory(const UPlayground_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}
