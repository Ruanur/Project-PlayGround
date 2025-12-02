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
};
