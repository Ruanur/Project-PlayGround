// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Playground_GridTypes.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Items/Fragment/Playground_ItemFragment.h"
#include "Playground_InventoryGrid.generated.h"



class UPlayground_ItemComponent;
class UCanvasPanel;
class UPlayground_GridSlot;
struct FPlayground_ItemManifest;
struct FPlayground_GridFragment;
class UPlayground_InventoryComponent;
class UPlayground_SlottedItem;
class UPlayground_HoverItem;
class UPlayground_ItemPopUp;
struct FGameplayTag;
enum class EPlayground_GridSlotState : uint8;

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	EPlayground_ItemCategory GetItemCategory() const { return ItemCategory; }
	FPlayground_SlotAvailabilityResult HasRoomForItem(const UPlayground_ItemComponent* ItemComponent);

	void PG_ShowCursor();
	void PG_HideCursor();
	void PG_SetOwningCanvas(UCanvasPanel* OwningCanvas);

	UFUNCTION()
	void AddItem(UPlayground_InventoryItem* Item);


private:
	TWeakObjectPtr<UPlayground_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<UCanvasPanel> OwningCanvasPanel;

	void ConstructGrid();
	FPlayground_SlotAvailabilityResult HasRoomForItem(const UPlayground_InventoryItem* Item);
	FPlayground_SlotAvailabilityResult HasRoomForItem(const FPlayground_ItemManifest& Manifest);
	void AddItemToIndices(const FPlayground_SlotAvailabilityResult& Result, UPlayground_InventoryItem* NewItem);
	bool MatchesCategory(const UPlayground_InventoryItem* Item) const;
	FVector2D GetDrawSize(const FPlayground_GridFragment* GridFragment) const;
	void SetSlottedItemImage(const UPlayground_SlottedItem* SlottedItem, const FPlayground_GridFragment* GridFragment, const FPlayground_ImageFragment* ImageFragment) const;
	void AddItemAtIndex(UPlayground_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);
	UPlayground_SlottedItem* CreateSlottedItem(UPlayground_InventoryItem* Item, 
		const bool bStackable, 
		const int32 StackAmount, 
		const FPlayground_GridFragment* GridFragment, 
		const FPlayground_ImageFragment* ImageFragment, 
		const int32 Index);
	void AddSlottedItemToCanvas(const int32 Index, const FPlayground_GridFragment* GridFragment, UPlayground_SlottedItem* SlottedItem) const;
	void UpdateGridSlots(UPlayground_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount);
	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const;
	bool HasRoomAtIndex(const UPlayground_GridSlot* GridSlot, 
		const FIntPoint& Dimensions,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		const int32 MaxStackSize);
	bool CheckSlotConstraints(const UPlayground_GridSlot* GridSlot, 
		const UPlayground_GridSlot* SubGridSlot, 
		const TSet<int32>& CheckedIndices, 
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		const int32 MaxStackSize) const;
	FIntPoint GetitemDimensions(const FPlayground_ItemManifest& Manifest) const;
	bool HasValidItem(const UPlayground_GridSlot* GridSlot) const;
	bool IsUpperLeftSlot(const UPlayground_GridSlot* GridSlot, const UPlayground_GridSlot* SubGridSlot) const;
	bool DoesItemTypeMatch(const UPlayground_InventoryItem* SubItem, const FGameplayTag& ItemType) const;
	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;
	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountFill, const UPlayground_GridSlot* GridSlot) const;
	int32 GetStackAmount(const UPlayground_GridSlot* GridSlot) const;
	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;
	void PickUp(UPlayground_InventoryItem* ClickedInventoryItem, const int32 GridIndex);
	void AssignHoverItem(UPlayground_InventoryItem* InventoryItem);
	void AssignHoverItem(UPlayground_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviouseGridIndex);
	void RemoveItemFromGrid(UPlayground_InventoryItem* InventoryItem, const int32 GridIndex);
	void PG_UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition);
	FIntPoint PG_CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	EPlayground_TileQuadrant PG_CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	void OnTileParametersUpdated(const FPlayground_TileParameters& Parameters);
	FIntPoint CalculateStartingCoordinate(const FIntPoint& Coordinate, const FIntPoint& Dimensions, const EPlayground_TileQuadrant Quadrant) const;
	FPlayground_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions);
	bool PG_CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& Location);
	void PG_HighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void PG_UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, EPlayground_GridSlotState GridSlotState);
	void PG_PutDownOnIndex(const int32 Index);
	void PG_ClearHoverItem();
	UUserWidget* PG_GetVisibleCursorWidget();
	UUserWidget* PG_GetHiddenCursorWidget();
	bool IsSameStackable(const UPlayground_InventoryItem* ClickedInventoryItem) const;
	void SwapWithHoverItem(UPlayground_InventoryItem* ClickedInventoryItem, const int32 GridIndex);
	bool PG_ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 HoveredStackCount, const int32 MaxStackSize) const;
	void PG_SwapStackCounts(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index);
	bool PG_ShouldConsumeHoverItemStacks(const int32 HoveredStackCount, const int32 RoomInClickedSlot) const;
	void PG_ConsumeHoverItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index);
	bool ShouldFillInStack(const int32 RoomInCilckedSlot, const int32 HoveredStackCount) const;
	void PG_FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index);
	void PG_CreateItemPopUp(const int32 GridIndex);

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlayground_ItemPopUp> ItemPopUpClass;

	UPROPERTY()
	TObjectPtr<UPlayground_ItemPopUp> ItemPopUp;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UUserWidget> VisibleCursorWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UUserWidget> HiddenCursorWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> VisibleCursorWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> HiddenCursorWidget;

	UFUNCTION()
	void AddStacks(const FPlayground_SlotAvailabilityResult& Result);

	UFUNCTION()
	void PG_OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotUnHovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void PG_OnPopUpMenuSplit(int32 SplitAmount, int32 Index);

	UFUNCTION()
	void PG_OnPopUpMenuDrop(int32 Index);

	UFUNCTION()
	void PG_OnPopUpMenuConsume(int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EPlayground_ItemCategory ItemCategory;

	UPROPERTY()
	TArray<TObjectPtr<UPlayground_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlayground_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlayground_SlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UPlayground_SlottedItem>> SlottedItems;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlayground_HoverItem> HoverItemClass;

	UPROPERTY()
	TObjectPtr<UPlayground_HoverItem> HoverItem;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D ItemPopUpOffset;

	FPlayground_TileParameters TileParameters;
	FPlayground_TileParameters LastTileParameters;

	// Index where an item would be placed if we click on the grid at a valid location
	int32 ItemDropIndex{ INDEX_NONE };
	FPlayground_SpaceQueryResult CurrentQueryResult;
	bool bMouseWithinCanvas;
	bool bLastMouseWithinCanvas;
	int32 LastHighlightedIndex;
	FIntPoint LastHighlightedDimensions;
};
