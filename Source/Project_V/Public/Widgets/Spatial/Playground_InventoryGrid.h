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
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	EPlayground_ItemCategory GetItemCategory() const { return ItemCategory; }
	FPlayground_SlotAvailabilityResult HasRoomForItem(const UPlayground_ItemComponent* ItemComponent);

	UFUNCTION()
	void AddItem(UPlayground_InventoryItem* Item);
private:
	TWeakObjectPtr<UPlayground_InventoryComponent> InventoryComponent;

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

	UFUNCTION()
	void AddStacks(const FPlayground_SlotAvailabilityResult& Result);

	UFUNCTION()
	void PG_OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

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

	FPlayground_TileParameters TileParameters;
	FPlayground_TileParameters LastTileParameters;
};
