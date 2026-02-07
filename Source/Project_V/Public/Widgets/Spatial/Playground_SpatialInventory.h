// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/PlaygroundWidgeBase.h"
#include "Widgets/GridSlots/Playground_EquippedGridSlot.h"
#include "Playground_SpatialInventory.generated.h"


class UPlayground_InventoryGrid;
class UWidgetSwitcher;
class UButton;
class UCanvasPanel;
class UPlayground_ItemDescription;
class UPlayground_HoverItem;

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_SpatialInventory : public UPlaygroundWidgeBase
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FPlayground_SlotAvailabilityResult HasRoomForItem(UPlayground_ItemComponent* ItemComponent) const override;
	virtual void OnItemHovered(UPlayground_InventoryItem* Item) override;
	virtual void OnItemUnHovered() override;
	virtual bool HasHoverItem() const override;
	virtual UPlayground_HoverItem* GetHoverItem() const override;
	virtual float GetTileSize() const override;
private:

	UPROPERTY()
	TArray<TObjectPtr<UPlayground_EquippedGridSlot>> EquippedGridSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayground_InventoryGrid> Grid_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayground_InventoryGrid> Grid_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayground_InventoryGrid> Grid_Craftables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craftables;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlayground_ItemDescription> ItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<UPlayground_ItemDescription> ItemDescription;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlayground_ItemDescription> EquippedItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<UPlayground_ItemDescription> EquippedItemDescription;

	FTimerHandle DescriptionTimer;
	FTimerHandle EquippedDescriptionTimer;

	UFUNCTION()
	void ShowEquippedItemDescription(UPlayground_InventoryItem* Item);

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DescriptionTimerDelay = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float EquippedDescriptionTimerDelay = 0.5f;

	UPlayground_ItemDescription* GetItemDescription();
	UPlayground_ItemDescription* GetEquippedItemDescription();

	UFUNCTION()
	void PG_ShowEquippables();

	UFUNCTION()
	void PG_ShowConsumables();

	UFUNCTION()
	void PG_ShowCraftables();

	UFUNCTION()
	void EquippedGridSlotClicked(UPlayground_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag);

	UFUNCTION()
	void EquippedSlottedItemClicked(UPlayground_EquippedSlottedItem* EquippedSlottedItem);

	void DisableButton(UButton* Button);
	void SetActiveGrid(UPlayground_InventoryGrid* Grid, UButton* Button);
	void SetItemDescriptionSizeAndPosition(UPlayground_ItemDescription* Description, UCanvasPanel* Canvas) const;
	void SetEquippedItemDescriptionSizeAndPosition(UPlayground_ItemDescription* Description, UPlayground_ItemDescription* EquippedDescription, UCanvasPanel* Canvas) const;
	bool CanEquipHoverItem(UPlayground_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag) const;
	UPlayground_EquippedGridSlot* FindSlotWithEquippedItem(UPlayground_InventoryItem* EquippedItem) const;
	void ClearSlotOfItem(UPlayground_EquippedGridSlot* EquippedGridSlot);
	void RemoveEquippedSlottedItem(UPlayground_EquippedSlottedItem* EquippedSlottedItem);
	void MakeEquippedSlottedItem(UPlayground_EquippedSlottedItem* EquippedSlottedItem, UPlayground_EquippedGridSlot* EquippedGridSlot, UPlayground_InventoryItem* ItemToEquip);
	void BroadcastSlotClickedDelegates(UPlayground_InventoryItem* ItemToEquip, UPlayground_InventoryItem* ItemToUnequip) const;

	TWeakObjectPtr<UPlayground_InventoryGrid> ActiveGrid;
};
