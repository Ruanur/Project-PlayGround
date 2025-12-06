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
	



	// Determine how many stacks to add.
	// For each Grid Slot:
	// If we don't have anymore to fill, break out of the loop early.
	// Is this index claimed yet?
	// Can the item fit here? (i.e. is it out of grid bounds?)
	// Is there room at this index? (i.e. are there other items in the way?)
	// Check any other important conditions - ForEach2D over a 2D range
		// Index claimed?
		// Has valid item?
		// Is this item the same type as the item we're trying to add?
		// If so, is this a stackable item?
		// If stackable, is this slot at the max stack size already?
	// How much to fill?
	// Update the amount left to fill
	// How much is the Remainder?
	
	// Determine if the item is stackable.
	// 아이템이 겹쳐지는지 (즉, 스택) 확인
	const FPlayground_StackableFragment* StackableFragment = Manifest.GetFragmentOfType<FPlayground_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;

	// 추가해야할 스택 수 계산
	// 각 그리드 슬롯 순회
	// 더 채울 아이템이 없다면 반복문 종료
	// 이 인덱스(슬롯)가 이미 사용 중인지 확인
	// 아이템이 이 위치에 배치될 수 있는지 확인. (그리드 범위를 벗어나지 않는다.)

	// 이 인덱스에 공간이 있는지 확인 (이때, 다른 아이템과 겹치지 않는다.)
	// 기타 필요한 조건들을 확인한다. - 2D 범위를 ForEach2D로 검사.
	// - 인덱스가 이미 점유되었는가?
	// - 유효한 아이템이 있는가?
	// - 현재 슬롯의 아이템이 추가하려는 아이템과 같은 종류인가?
	// - 같은 종류라면, 스택 가능한 타입인가?
	// - 스택 가능한 경우, 이미 최대 스택 수에 도달했는가?

	// 이번 슬롯에 얼마나 채울 수 있는지 계산한다. 
	// 남은 채울 양을 업데이트. 
	// 채우고 난 뒤 남는(채우지 못한) 수량을 계산

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
		UpdateGridSlots(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
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
	SlottedItem->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->PG_UpdateStackCount(StackUpdateAmount);

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

void UPlayground_InventoryGrid::UpdateGridSlots(UPlayground_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));

	if (bStackableItem)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}

	const FPlayground_GridFragment* GridFragment = GetFragment<FPlayground_GridFragment>(NewItem, FragmentTags::GridFragment);
	const FIntPoint Dimension = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);


	UPlayground_InventoryStatics::ForEach2D(GridSlots, Index ,Dimension, Columns, [&](UPlayground_GridSlot* GridSlot)
		{
			GridSlot->SetInventoryItem(NewItem);
			GridSlot->SetUpperLeftIndex(Index);
			GridSlot->PG_SetOccupiedTexture();
			GridSlot->SetAvailable(false);
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


