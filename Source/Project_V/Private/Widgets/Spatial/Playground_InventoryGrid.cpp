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
#include "Widgets/HoverItem/Playground_HoverItem.h"

#include "PlaygroundDebugHelper.h"



void UPlayground_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Debug::Print(TEXT("UPlayground_InventoryGrid::NativeOnInitialized Called"));

	ConstructGrid();

	InventoryComponent = UPlayground_InventoryStatics::PG_GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
	InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::AddStacks);
}

void UPlayground_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D CanvasPosition = UPlayground_WidgetUtils::PG_GetWidgetPosition(CanvasPanel);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	PG_UpdateTileParameters(CanvasPosition, MousePosition);
}

void UPlayground_InventoryGrid::PG_UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition)
{

	// if mouse not in canvas panel, return.
	// Calculate the tile quadrant, tile index, and coordinates
	const FIntPoint HoveredTileCoordinates = PG_CalculateHoveredCoordinates(CanvasPosition, MousePosition);

	LastTileParameters = TileParameters;
	TileParameters.TileCoordinates = HoveredTileCoordinates;
	TileParameters.TileIndex = UPlayground_WidgetUtils::PG_GetIndexFromPosition(HoveredTileCoordinates, Columns);
	TileParameters.TileQuadrant = PG_CalculateTileQuadrant(CanvasPosition, MousePosition);

	// Handle highlight/unhighlight of the grid slots
	OnTileParametersUpdated(TileParameters);
}

void UPlayground_InventoryGrid::OnTileParametersUpdated(const FPlayground_TileParameters& Parameters)
{
	if (!IsValid(HoverItem)) return;

	// Get Hover Item's dimensions
	// calculate the starting coordinate for highlighting
	// check hover position
		// in the grid bounds?
		// any items in the way?
		// if so, is there only one item in the way? (can we swap?)
		

}

FIntPoint UPlayground_InventoryGrid::PG_CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	return FIntPoint
	{
		static_cast<int32>(FMath::FloorToInt((MousePosition.X - CanvasPosition.X) / TileSize)),
		static_cast<int32>(FMath::FloorToInt((MousePosition.Y - CanvasPosition.Y) / TileSize))
	};
}

EPlayground_TileQuadrant UPlayground_InventoryGrid::PG_CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	// calculate relative position within the current tile
	const float TileLocalX = FMath::Fmod(MousePosition.X - CanvasPosition.X, TileSize);
	const float TileLocalY = FMath::Fmod(MousePosition.Y - CanvasPosition.Y, TileSize);

	// Determine which quadrant the mouse is in
	const bool bIsTop = TileLocalY < TileSize / 2.f; // top is Y is in the upper half
	const bool bIsLeft = TileLocalX < TileSize / 2.f; // Left if X is in the left half 

	EPlayground_TileQuadrant HoveredTileQuadrant;
	if (bIsTop && bIsLeft) HoveredTileQuadrant = EPlayground_TileQuadrant::TopLeft;
	else if (bIsTop && !bIsLeft) HoveredTileQuadrant = EPlayground_TileQuadrant::TopRight;
	else if (!bIsTop && bIsLeft) HoveredTileQuadrant = EPlayground_TileQuadrant::BottomLeft;
	else if (!bIsTop && !bIsLeft) HoveredTileQuadrant = EPlayground_TileQuadrant::BottomRight;

	return HoveredTileQuadrant;
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
	
	
	// Determine if the item is stackable.
	// 아이템이 겹쳐지는지 (즉, 스택) 확인
	const FPlayground_StackableFragment* StackableFragment = Manifest.GetFragmentOfType<FPlayground_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;

	// Determine how many stacks to add.
	// 추가해야할 스택 수 계산
	const int32 MaxStackSize = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;

	TSet<int32> CheckedIndices;
	// 각 그리드 슬롯 순회
	// For each Grid Slot:
	for (const auto& GridSlot : GridSlots)
	{
		// If we don't have anymore to fill, break out of the loop early.
		// 더 채울 아이템이 없다면 반복문 종료
		if (AmountToFill == 0) break;

		// Is this index claimed yet?
		// 이 인덱스(슬롯)가 이미 사용 중인지 확인
		if (IsIndexClaimed(CheckedIndices, GridSlot->GetIndex())) continue;

		// Is the Item in grid Bounds?
		if (!IsInGridBounds(GridSlot->GetIndex(), GetitemDimensions(Manifest))) continue;

		// Can the item fit here? (i.e. is it out of grid bounds?)
		// 아이템이 이 위치에 배치될 수 있는지 확인. (그리드 범위를 벗어나지 않는다.)
		TSet<int32> TentativelyClaimed;
		if (!HasRoomAtIndex(GridSlot, GetitemDimensions(Manifest), CheckedIndices, TentativelyClaimed, Manifest.GetItemType(), MaxStackSize))
		{
			continue;
		}


		// How much to fill?
		// 이번 슬롯에 얼마나 채울 수 있는지 계산한다. 
		const int32 AmountToFillInSlot = DetermineFillAmountForSlot(Result.bStackable, MaxStackSize, AmountToFill, GridSlot);
		if (AmountToFillInSlot == 0) continue;


		CheckedIndices.Append(TentativelyClaimed);

		// Update the amount left to fill
		// 남은 채울 양을 업데이트. 
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			FPlayground_SlotAvailability{
				HasValidItem(GridSlot) ? GridSlot->GetUpperLeftIndex() : GridSlot->GetIndex(),
				Result.bStackable ? AmountToFillInSlot : 0,
				HasValidItem(GridSlot)
			}
		);

		AmountToFill -= AmountToFillInSlot;

		// How much is the Remainder?
		// 채우고 난 뒤 남는(채우지 못한) 수량을 계산
		Result.Remainder = AmountToFill;

		if (AmountToFill == 0) return Result;
	}
	

	return Result;
}

bool UPlayground_InventoryGrid::HasRoomAtIndex(const UPlayground_GridSlot* GridSlot,
												const FIntPoint& Dimensions,
												const TSet<int32>& CheckedIndices,
												TSet<int32>& OutTentativelyClaimed,
												const FGameplayTag& ItemType,
												const int32 MaxStackSize)
{
	// Is there room at this index? (i.e. are there other items in the way?)
	// 이 인덱스에 공간이 있는지 확인 (이때, 다른 아이템과 겹치지 않는다.)
	bool bHasRoomAtIndex = true;
	UPlayground_InventoryStatics::ForEach2D(GridSlots, GridSlot->GetIndex(), Dimensions, Columns, [&](const UPlayground_GridSlot* SubGridSlot)
	{
		if (CheckSlotConstraints(GridSlot, SubGridSlot, CheckedIndices, OutTentativelyClaimed, ItemType, MaxStackSize))
		{
				OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		}
		else
		{
			bHasRoomAtIndex = false;
		}
	});

	return bHasRoomAtIndex;
}

bool UPlayground_InventoryGrid::CheckSlotConstraints(const UPlayground_GridSlot* GridSlot,
													const UPlayground_GridSlot* SubGridSlot,
													const TSet<int32>& CheckedIndices, 
													TSet<int32>& OutTentativelyClaimed,
													const FGameplayTag& ItemType,
													const int32 MaxStackSize) const
{
	// Check any other important conditions - ForEach2D over a 2D range
	// 기타 필요한 조건들을 확인한다. - 2D 범위를 ForEach2D로 검사.
	// Index claimed?
	// - 인덱스가 이미 점유되었는가?
	if (IsIndexClaimed(CheckedIndices, SubGridSlot->GetIndex())) return false;

	// Has valid item?
	// - 유효한 아이템이 있는가?
	if (!HasValidItem(SubGridSlot))
	{
		OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		return true;
	}

	// Is this Grid Slot an upper left slot?
	// 이 그리드 슬롯이 아이템의 좌상단(Upper Left) 슬롯인가?
	if (!IsUpperLeftSlot(GridSlot, SubGridSlot)) return false;

	
	// If so, is this a stackable item?
	// - 같은 종류라면, 스택 가능한 타입인가?
	const UPlayground_InventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();
	if (!SubItem->IsStackable()) return false;

	// Is this item the same type as the item we're trying to add?
	// - 현재 슬롯의 아이템이 추가하려는 아이템과 같은 종류인가?
	if (!DoesItemTypeMatch(SubItem, ItemType)) return false;

	// If stackable, is this slot at the max stack size already?
	// - 스택 가능한 경우, 이미 최대 스택 수에 도달했는가?
	if (GridSlot->GetStackCount() >= MaxStackSize) return false;

	return true;
}

FIntPoint UPlayground_InventoryGrid::GetitemDimensions(const FPlayground_ItemManifest& Manifest) const
{
	const FPlayground_GridFragment* GridFragment = Manifest.GetFragmentOfType<FPlayground_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
}

bool UPlayground_InventoryGrid::HasValidItem(const UPlayground_GridSlot* GridSlot) const
{
	return GridSlot->GetInventoryItem().IsValid();
}

bool UPlayground_InventoryGrid::IsUpperLeftSlot(const UPlayground_GridSlot* GridSlot, const UPlayground_GridSlot* SubGridSlot) const
{
	return SubGridSlot->GetUpperLeftIndex() == GridSlot->GetIndex();
}

bool UPlayground_InventoryGrid::DoesItemTypeMatch(const UPlayground_InventoryItem* SubItem, const FGameplayTag& ItemType) const
{
	return SubItem->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
}

bool UPlayground_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num()) return false;
	
	const int32 EndColumn = (StartIndex % Columns) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;
	return EndColumn <= Columns && EndRow <= Rows;
}

int32 UPlayground_InventoryGrid::DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountFill, const UPlayground_GridSlot* GridSlot) const
{
	// Calculate room in the Slot
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);

	// if Stackable, need the minimum between AmountFill and RoomInSlot
	return bStackable ? FMath::Min(AmountFill, RoomInSlot) : 1;
}

int32 UPlayground_InventoryGrid::GetStackAmount(const UPlayground_GridSlot* GridSlot) const
{
	int32 CurrentSlotStackCount = GridSlot->GetStackCount();
	// If we are at a slot that doesn't hold the stack count. we must get the actual stack count
	if (const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex(); UpperLeftIndex != INDEX_NONE)
	{
		UPlayground_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
		CurrentSlotStackCount = UpperLeftGridSlot->GetStackCount();
	}

	return CurrentSlotStackCount;
}

bool UPlayground_InventoryGrid::IsRightClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UPlayground_InventoryGrid::IsLeftClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

void UPlayground_InventoryGrid::PickUp(UPlayground_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	// Assign the hover item
	AssignHoverItem(ClickedInventoryItem, GridIndex, GridIndex);

	// Remove clicked item from the grid
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
}

void UPlayground_InventoryGrid::AssignHoverItem(UPlayground_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviouseGridIndex)
{
	AssignHoverItem(InventoryItem);

	HoverItem->SetPreviousGridIndex(PreviouseGridIndex);
	HoverItem->UpdateStackCount(InventoryItem->IsStackable() ? GridSlots[GridIndex]->GetStackCount() : 0);
}

void UPlayground_InventoryGrid::RemoveItemFromGrid(UPlayground_InventoryItem* InventoryItem, const int32 GridIndex)
{
	const FPlayground_GridFragment* GridFragment = GetFragment<FPlayground_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	UPlayground_InventoryStatics::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns, [&](UPlayground_GridSlot* GridSlot) 
		{
			GridSlot->SetInventoryItem(nullptr);
			GridSlot->SetUpperLeftIndex(INDEX_NONE);
			GridSlot->PG_SetUnoccupiedTexture();
			GridSlot->SetAvailable(true);
			GridSlot->SetStackCount(0);
		});

	if (SlottedItems.Contains(GridIndex))
	{
		TObjectPtr<UPlayground_SlottedItem> FoundSlottedItem;
		SlottedItems.RemoveAndCopyValue(GridIndex, FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}
}



void UPlayground_InventoryGrid::AssignHoverItem(UPlayground_InventoryItem* InventoryItem)
{
	if (!IsValid(HoverItem))
	{
		HoverItem = CreateWidget<UPlayground_HoverItem>(GetOwningPlayer(), HoverItemClass);
	}

	const FPlayground_GridFragment* GridFragment = GetFragment<FPlayground_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	const FPlayground_ImageFragment* ImageFragment = GetFragment<FPlayground_ImageFragment>(InventoryItem, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return;

	const FVector2D DrawSize = GetDrawSize(GridFragment);

	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ImageFragment->GetIcon());
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	IconBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);

	HoverItem->SetImageBrush(IconBrush);
	HoverItem->SetGridDimensions(GridFragment->GetGridSize());
	HoverItem->SetInventoryItem(InventoryItem);
	HoverItem->SetIsStackable(InventoryItem->IsStackable());

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, HoverItem);
}



void UPlayground_InventoryGrid::AddStacks(const FPlayground_SlotAvailabilityResult& Result)
{
	if (!MatchesCategory(Result.Item.Get())) return;

	for (const auto& Availability : Result.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex)
		{
			const auto& GridSlot = GridSlots[Availability.Index];
			const auto& SlottedItem = SlottedItems.FindChecked(Availability.Index);
			SlottedItem->PG_UpdateStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
		}
		else
		{
			AddItemAtIndex(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
			UpdateGridSlots(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
		}
	}
}

void UPlayground_InventoryGrid::PG_OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	check(GridSlots.IsValidIndex(GridIndex));
	UPlayground_InventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem().Get();

	if (!IsValid(HoverItem))
	{
		// TODO: PickUp - Assign the hover item, and remove the slotted item from the grid.
		PickUp(ClickedInventoryItem, GridIndex);

		
	}
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
	SlottedItem->OnSlottedItemClicked.AddDynamic(this, &ThisClass::PG_OnSlottedItemClicked);

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

bool UPlayground_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const
{
	return CheckedIndices.Contains(Index);
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


