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

	FTimerHandle DescriptionTimer;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DescriptionTimerDelay = 0.5;

	UPlayground_ItemDescription* GetItemDescription();

	UFUNCTION()
	void PG_ShowEquippables();

	UFUNCTION()
	void PG_ShowConsumables();

	UFUNCTION()
	void PG_ShowCraftables();

	UFUNCTION()
	void EquippedGridSlotClicked(UPlayground_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag);

	UFUNCTION()
	void EquippedSlottedItemClicked(UPlayground_EquippedSlottedItem* SlottedItem);

	void DisableButton(UButton* Button);
	void SetActiveGrid(UPlayground_InventoryGrid* Grid, UButton* Button);
	void SetItemDescriptionSizeAndPosition(UPlayground_ItemDescription* Description, UCanvasPanel* Canvas) const;
	bool CanEquipHoverItem(UPlayground_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag) const;

	TWeakObjectPtr<UPlayground_InventoryGrid> ActiveGrid;
};
