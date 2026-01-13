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
#include "Widgets/ItemPopUp/Playground_ItemPopUp.h"

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

	if (PG_CursorExitedCanvas(CanvasPosition, UPlayground_WidgetUtils::PG_GetWidgetSize(CanvasPanel), MousePosition))
	{
		return;
	}

	PG_UpdateTileParameters(CanvasPosition, MousePosition);
}

void UPlayground_InventoryGrid::PG_UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition)
{
	// if mouse not in canvas panel, return.
	if (!bMouseWithinCanvas) return;

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
	const FIntPoint Dimensions = HoverItem->GetGridDimensions();


	// calculate the starting coordinate for highlighting
	const FIntPoint StartingCoordinate = CalculateStartingCoordinate(Parameters.TileCoordinates, Dimensions, Parameters.TileQuadrant);
	ItemDropIndex = UPlayground_WidgetUtils::PG_GetIndexFromPosition(StartingCoordinate, Columns);


	CurrentQueryResult = CheckHoverPosition(StartingCoordinate, Dimensions);

	if (CurrentQueryResult.bHasSpace)
	{
		PG_HighlightSlots(ItemDropIndex, Dimensions);
		return;
	}
	PG_UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);

	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		// TODO: There's a single item in this space.  We can swap or add Stacks
		const FPlayground_GridFragment* GridFragment = GetFragment<FPlayground_GridFragment>(CurrentQueryResult.ValidItem.Get(), FragmentTags::GridFragment);
		if (!GridFragment) return;

		ChangeHoverType(CurrentQueryResult.UpperLeftIndex, GridFragment->GetGridSize(), EPlayground_GridSlotState::GrayedOut);
	}
}

FPlayground_SpaceQueryResult UPlayground_InventoryGrid::CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions) 
{
	FPlayground_SpaceQueryResult Result;

	// check hover position

	// in the grid bounds?
	if (!IsInGridBounds(UPlayground_WidgetUtils::PG_GetIndexFromPosition(Position, Columns), Dimensions)) return Result;

	Result.bHasSpace = true;

	// If more than one of the indices is occupied with the same item, we need to see if they all have the same upper left index.
	TSet<int32> OccupiedUpperLeftIndices;
	UPlayground_InventoryStatics::ForEach2D(GridSlots, UPlayground_WidgetUtils::PG_GetIndexFromPosition(Position, Columns), Dimensions, Columns, [&](const UPlayground_GridSlot* GridSlot) 
		{
			if (GridSlot->GetInventoryItem().IsValid())
			{
				OccupiedUpperLeftIndices.Add(GridSlot->GetUpperLeftIndex());
				Result.bHasSpace = false;
			}
		});

	// if so, is there only one item in the way? (can we swap?)
	if (OccupiedUpperLeftIndices.Num() == 1) // Single item at position - it's valid for swapping/combining
	{
		const int32 Index = *OccupiedUpperLeftIndices.CreateConstIterator();
		Result.ValidItem = GridSlots[Index]->GetInventoryItem();
		Result.UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	}

	return Result;
}

bool UPlayground_InventoryGrid::PG_CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& Location)
{
	bLastMouseWithinCanvas = bMouseWithinCanvas;
	bMouseWithinCanvas = UPlayground_WidgetUtils::PG_IsWithinBounds(BoundaryPos, BoundarySize, Location);
	if (!bMouseWithinCanvas && bLastMouseWithinCanvas)
	{
		// TODO: UnhighlightSlots()
		PG_UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
		return true;
	}
	return false;
}

void UPlayground_InventoryGrid::PG_HighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	if (!bMouseWithinCanvas) return;
	PG_UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);

	UPlayground_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UPlayground_GridSlot* GridSlot) 
		{
			GridSlot->PG_SetOccupiedTexture();
		});

	LastHighlightedDimensions = Dimensions;
	LastHighlightedIndex = Index;
}

void UPlayground_InventoryGrid::PG_UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	UPlayground_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UPlayground_GridSlot* GridSlot)
		{
			if (GridSlot->IsAvailable())
			{
				GridSlot->PG_SetUnoccupiedTexture();
			}
			else
			{
				GridSlot->PG_SetOccupiedTexture();
			}
		});
}

void UPlayground_InventoryGrid::ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, EPlayground_GridSlotState GridSlotState)
{
	PG_UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	UPlayground_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [State = GridSlotState](UPlayground_GridSlot* GridSlot)
		{
			switch (State)
			{
			case EPlayground_GridSlotState::Occupied:
				GridSlot->PG_SetOccupiedTexture();
				break;
			case EPlayground_GridSlotState::Unoccupied:
				GridSlot->PG_SetUnoccupiedTexture();
				break;
			case EPlayground_GridSlotState::GrayedOut:
				GridSlot->PG_SetGrayedOutTexture();
				break;
			case EPlayground_GridSlotState::Selected:
				GridSlot->PG_SetSelectedTexture();
				break;
			}
		});

	LastHighlightedIndex = Index;
	LastHighlightedDimensions = Dimensions;
}

FIntPoint UPlayground_InventoryGrid::CalculateStartingCoordinate(const FIntPoint& Coordinate, const FIntPoint& Dimensions, const EPlayground_TileQuadrant Quadrant) const
{
	const int32 HasEvenWidth = Dimensions.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight = Dimensions.Y % 2 == 0 ? 1 : 0;

	FIntPoint StartingCoord;

	switch (Quadrant)
	{
	case EPlayground_TileQuadrant::TopLeft:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;

	case EPlayground_TileQuadrant::TopRight:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;

	case EPlayground_TileQuadrant::BottomLeft:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;

	case EPlayground_TileQuadrant::BottomRight:
		StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
		StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;
	default:
		Debug::Print(TEXT("Invalid Quadrant."));
		return FIntPoint(-1, -1);
	}

	return StartingCoord;
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

	// ISSUE:
	// Right-click behavior is currently dependent on HoverItem being valid.
	// If HoverItem is null, this function returns early due to PickUp(),
	// preventing the right-click popup from ever being created.
	//
	// As a result:
	// - Right-clicking an item without holding another item does NOT show the popup.
	// - The popup only appears when an item is already picked up (HoverItem is valid).
	//
	// This tightly couples input handling with inventory state,
	// which leads to unintuitive UX and hard-to-maintain logic.
	
	// IMPROVEMENT:
	// Right-click input should always be handled first,
	// regardless of whether an item is currently being hovered or dragged.
	//
	// This ensures consistent and predictable popup behavior.
	if (IsRightClick(MouseEvent))
	{
		PG_CreateItemPopUp(GridIndex);
		return;
	}

	// Left-Click behavior below
	// If no Item is currently begin dragged, pick up the clicked item.

	if (!IsValid(HoverItem))
	{
		// TODO: PickUp - Assign the hover item, and remove the slotted item from the grid.
		PickUp(ClickedInventoryItem, GridIndex);
		return;	
	}

	// Do the hovered item and cilcked inventory item share a type, and are they stackable?
	if (IsSameStackable(ClickedInventoryItem))
	{
		const int32 ClickedStackCount = GridSlots[GridIndex]->GetStackCount();
		const FPlayground_StackableFragment* StackableFragment = ClickedInventoryItem->GetItemManifest().GetFragmentOfType<FPlayground_StackableFragment>();
		const int32 MaxStackSize = StackableFragment->GetMaxStackSize();
		const int32 RoomInClickedSlot = MaxStackSize - ClickedStackCount;
		const int32 HoveredStackCount = HoverItem->GetStackCount();


		// Should we swap their stack counts? (Room in the clicked slot == 0 && HoveredStackCount < MaxStackSize)
		if (PG_ShouldSwapStackCounts(RoomInClickedSlot, HoveredStackCount, MaxStackSize))
		{
			// TODO: Swap Stack Counts
			PG_SwapStackCounts(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}

		// Should we consume the hover item's stacks? (Room in the clicked slot >= HoveredStackCount)
		if (PG_ShouldConsumeHoverItemStacks(HoveredStackCount, RoomInClickedSlot))
		{
			// TODO: ComsumeHoverItemSlots
			PG_ConsumeHoverItemStacks(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}

		// Should we fill in the stacks of the clicked item? (and not consume the hover item)
		if (ShouldFillInStack(RoomInClickedSlot, HoveredStackCount))
		{
			PG_FillInStack(RoomInClickedSlot, HoveredStackCount - RoomInClickedSlot, GridIndex);
			return;
		}

		
		
		// Clicked Slot is already full - do nothing (maybe play a sound?)
		if (RoomInClickedSlot == 0)
		{
			return;
		}
	}

	// Swap with the hover item.
	SwapWithHoverItem(ClickedInventoryItem, GridIndex);
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
			GridSlot->GridSlotClicked.AddDynamic(this, &ThisClass::OnGridSlotClicked);
			GridSlot->GridSlotHovered.AddDynamic(this, &ThisClass::OnGridSlotHovered);
			GridSlot->GridSlotUnHovered.AddDynamic(this, &ThisClass::OnGridSlotUnHovered);
		}
	}
}

void UPlayground_InventoryGrid::OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (!IsValid(HoverItem)) return;
	if (!GridSlots.IsValidIndex(ItemDropIndex)) return;

	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		PG_OnSlottedItemClicked(CurrentQueryResult.UpperLeftIndex, MouseEvent);
		return;
	}

	auto GridSlot = GridSlots[ItemDropIndex];
	if (!GridSlot->GetInventoryItem().IsValid())
	{
		// TODO: Put item down at this index.
		PG_PutDownOnIndex(ItemDropIndex);
	}
}

void UPlayground_InventoryGrid::PG_PutDownOnIndex(const int32 Index)
{
	AddItemAtIndex(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount());
	UpdateGridSlots(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount());
	PG_ClearHoverItem();
}

void UPlayground_InventoryGrid::PG_ClearHoverItem()
{
	if (!IsValid(HoverItem)) return;

	HoverItem->SetInventoryItem(nullptr);
	HoverItem->SetIsStackable(false);
	HoverItem->SetPreviousGridIndex(INDEX_NONE);
	HoverItem->UpdateStackCount(0);
	HoverItem->SetImageBrush(FSlateNoResource());

	HoverItem->RemoveFromParent();
	HoverItem = nullptr;

	// TODO: Show Mouse Cursor
	PG_ShowCursor();
}

UUserWidget* UPlayground_InventoryGrid::PG_GetVisibleCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;
	if (!IsValid(VisibleCursorWidget))
	{
		VisibleCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), VisibleCursorWidgetClass);
	}

	return VisibleCursorWidget;
}

UUserWidget* UPlayground_InventoryGrid::PG_GetHiddenCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;
	if (!IsValid(HiddenCursorWidget))
	{
		HiddenCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), HiddenCursorWidgetClass);
	}

	return HiddenCursorWidget;
}

bool UPlayground_InventoryGrid::IsSameStackable(const UPlayground_InventoryItem* ClickedInventoryItem) const
{
	const bool bIsSameItem = ClickedInventoryItem == HoverItem->GetInventoryItem();
	const bool bIsStackable = ClickedInventoryItem->IsStackable();
	return bIsSameItem && bIsStackable && HoverItem->GetItemType().MatchesTagExact(ClickedInventoryItem->GetItemManifest().GetItemType());
}

void UPlayground_InventoryGrid::SwapWithHoverItem(UPlayground_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	if (!IsValid(HoverItem)) return;

	UPlayground_InventoryItem* TempInventoryItem = HoverItem->GetInventoryItem();
	const int32 TempStackCount = HoverItem->GetStackCount();
	const bool bTempIsStackable = HoverItem->IsStackable();

	// Keep the same pervious grid Index
	AssignHoverItem(ClickedInventoryItem, GridIndex, HoverItem->GetPreviousGridIndex());
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
	AddItemAtIndex(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
	UpdateGridSlots(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
}

bool UPlayground_InventoryGrid::PG_ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 HoveredStackCount, const int32 MaxStackSize) const
{
	return RoomInClickedSlot == 0 && HoveredStackCount < MaxStackSize;
}

void UPlayground_InventoryGrid::PG_SwapStackCounts(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index)
{
	UPlayground_GridSlot* GridSlot = GridSlots[Index];
	GridSlot->SetStackCount(HoveredStackCount);
	
	UPlayground_SlottedItem* ClickedSlottedItem = SlottedItems.FindChecked(Index);
	ClickedSlottedItem->PG_UpdateStackCount(HoveredStackCount);

	HoverItem->UpdateStackCount(ClickedStackCount);
}

bool UPlayground_InventoryGrid::PG_ShouldConsumeHoverItemStacks(const int32 HoveredStackCount, const int32 RoomInClickedSlot) const
{
	return RoomInClickedSlot >= HoveredStackCount;
}

void UPlayground_InventoryGrid::PG_ConsumeHoverItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index)
{
	const int32 AmountToTransfer = HoveredStackCount;
	const int32 NewClickedStackCount = ClickedStackCount + AmountToTransfer;

	GridSlots[Index]->SetStackCount(NewClickedStackCount);
	SlottedItems.FindChecked(Index)->PG_UpdateStackCount(NewClickedStackCount);
	PG_ClearHoverItem();
	PG_ShowCursor();

	const FPlayground_GridFragment* GridFragment = GridSlots[Index]->GetInventoryItem()->GetItemManifest().GetFragmentOfType<FPlayground_GridFragment>();
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
	PG_HighlightSlots(Index, Dimensions);
}

bool UPlayground_InventoryGrid::ShouldFillInStack(const int32 RoomInCilckedSlot, const int32 HoveredStackCount) const
{
	return RoomInCilckedSlot < HoveredStackCount;
}

void UPlayground_InventoryGrid::PG_FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index)
{
	UPlayground_GridSlot* GridSlot = GridSlots[Index];
	const int32 NewStackCount = GridSlot->GetStackCount() + FillAmount;

	GridSlot->SetStackCount(NewStackCount);

	UPlayground_SlottedItem* ClickedSlottedItem = SlottedItems.FindChecked(Index);
	ClickedSlottedItem->PG_UpdateStackCount(NewStackCount);

	HoverItem->UpdateStackCount(Remainder);
}

void UPlayground_InventoryGrid::PG_CreateItemPopUp(const int32 GridIndex)
{
	UPlayground_InventoryItem* RightClickedItem = GridSlots[GridIndex]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem)) return;
	if (IsValid(GridSlots[GridIndex]->PG_GetItemPopUp())) return;

	ItemPopUp = CreateWidget<UPlayground_ItemPopUp>(this, ItemPopUpClass);
	GridSlots[GridIndex]->SetItemPopUp(ItemPopUp);

	OwningCanvasPanel->AddChild(ItemPopUp);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemPopUp);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	
	// SetPosition >> Position adjustment for the item popup
	CanvasSlot->SetPosition(MousePosition - ItemPopUpOffset);
	CanvasSlot->SetSize(ItemPopUp->PG_GetBoxSize());

	const int32 SliderMax = GridSlots[GridIndex]->GetStackCount() - 1;
	if (RightClickedItem->IsStackable() && SliderMax > 0)
	{
		ItemPopUp->OnSplit.BindDynamic(this, &ThisClass::PG_OnPopUpMenuSplit);
		ItemPopUp->SetSliderParams(SliderMax, FMath::Max(1, GridSlots[GridIndex]->GetStackCount() / 2));
	}
	else
	{
		ItemPopUp->PG_CollapseSplitButton();
	}

	ItemPopUp->OnDrop.BindDynamic(this, &ThisClass::PG_OnPopUpMenuDrop);

	if (RightClickedItem->IsConsumable())
	{
		ItemPopUp->OnConsume.BindDynamic(this, &ThisClass::PG_OnPopUpMenuConsume);
	}
	else
	{
		ItemPopUp->PG_CollapseSplitButton();
	}
	
}

void UPlayground_InventoryGrid::PG_DropItem()
{
	if (!IsValid(HoverItem)) return;
	if (!IsValid(HoverItem->GetInventoryItem())) return;

	// TODO: Tell the server to actually drop the item

	PG_ClearHoverItem();
	PG_ShowCursor();
}


void UPlayground_InventoryGrid::PG_ShowCursor()
{
	if (!IsValid(GetOwningPlayer())) return;

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, PG_GetVisibleCursorWidget());
}

void UPlayground_InventoryGrid::PG_HideCursor()
{
	if (!IsValid(GetOwningPlayer())) return;

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, PG_GetHiddenCursorWidget());
}

void UPlayground_InventoryGrid::PG_SetOwningCanvas(UCanvasPanel* OwningCanvas)
{
	OwningCanvasPanel = OwningCanvas;
}

void UPlayground_InventoryGrid::OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem)) return;

	UPlayground_GridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->PG_SetOccupiedTexture();
	}
}

void UPlayground_InventoryGrid::OnGridSlotUnHovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem)) return;

	UPlayground_GridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->PG_SetUnoccupiedTexture();
	}
}

void UPlayground_InventoryGrid::PG_OnPopUpMenuSplit(int32 SplitAmount, int32 Index)
{
	UPlayground_InventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem)) return;
	if (!RightClickedItem->IsStackable()) return;

	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UPlayground_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	const int32 StackCount = UpperLeftGridSlot->GetStackCount();
	const int32 NewStackCount = StackCount - SplitAmount;

	UpperLeftGridSlot->SetStackCount(NewStackCount);
	SlottedItems.FindChecked(UpperLeftIndex)->PG_UpdateStackCount(NewStackCount);

	AssignHoverItem(RightClickedItem, UpperLeftIndex, UpperLeftIndex);
	HoverItem->UpdateStackCount(SplitAmount);
}

void UPlayground_InventoryGrid::PG_OnPopUpMenuDrop(int32 Index)
{
	UPlayground_InventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem)) return;

	PickUp(RightClickedItem, Index);
	PG_DropItem();
}

void UPlayground_InventoryGrid::PG_OnPopUpMenuConsume(int32 Index)
{

}

bool UPlayground_InventoryGrid::MatchesCategory(const UPlayground_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}


