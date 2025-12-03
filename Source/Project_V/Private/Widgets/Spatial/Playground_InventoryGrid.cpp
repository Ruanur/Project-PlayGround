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
#include "Widgets/SlottedItems/Playground_SlottedItem.h"

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
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemAtIndex(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
		UpdateGridSlots(NewItem, Availability.Index);
	}

	// Create a widget to add to the grid
	// Store the new widget in a container
}

void UPlayground_InventoryGrid::AddItemAtIndex(UPlayground_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount)
{
	const FPlayground_GridFragment* GridFragment = GetFragment<FPlayground_GridFragment>(Item, FragmentTags::GridFragment);
	const FPlayground_ImageFragment* ImageFragment = GetFragment<FPlayground_ImageFragment>(Item, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return;

	UPlayground_SlottedItem* SlottedItem = CreateSlottedItem(Item, bStackable, StackAmount, GridFragment, ImageFragment, Index);

	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

	SlottedItems.Add(Index, SlottedItem);

}

UPlayground_SlottedItem* UPlayground_InventoryGrid::CreateSlottedItem(UPlayground_InventoryItem* Item, const bool bStackable, const int32 StackAmount, const FPlayground_GridFragment* GridFragment, const FPlayground_ImageFragment* ImageFragment, const int32 Index)
{
	UPlayground_SlottedItem* SlottedItem = CreateWidget<UPlayground_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(Item);
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	SlottedItem->SetGridIndex(Index);

	return SlottedItem;
}

void UPlayground_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FPlayground_GridFragment* GridFragment, UPlayground_SlottedItem* SlottedItem) const
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPos = UPlayground_WidgetUtils::PG_GetPositionFromIndex(Index, Columns) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());
	CanvasSlot->SetPosition(DrawPosWithPadding);
}

void UPlayground_InventoryGrid::UpdateGridSlots(UPlayground_InventoryItem* NewItem, const int32 Index)
{
	check(GridSlots.IsValidIndex(Index));

	const FPlayground_GridFragment* GridFragment = GetFragment<FPlayground_GridFragment>(NewItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	const FIntPoint Dimension = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);


	UPlayground_InventoryStatics::ForEach2D(GridSlots, Index ,Dimension, Columns, [](UPlayground_GridSlot* GridSlot)
	{
		GridSlot->PG_SetOccupiedTexture();
	});
}

FVector2D UPlayground_InventoryGrid::GetDrawSize(const FPlayground_GridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return GridFragment->GetGridSize() * IconTileWidth;
}

void UPlayground_InventoryGrid::SetSlottedItemImage(const UPlayground_SlottedItem* SlottedItem, const FPlayground_GridFragment* GridFragment, const FPlayground_ImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
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


