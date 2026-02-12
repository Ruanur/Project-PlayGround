// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/GridSlots/Playground_GridSlot.h"
#include "GameplayTagContainer.h"
#include "Playground_EquippedGridSlot.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClicked, UPlayground_EquippedGridSlot*, GridSlot, const FGameplayTag&, EquipmentTypeTag);

class UImage;
class UPlayground_EquippedSlottedItem;
class UOverlay;

UCLASS()
class PROJECT_V_API UPlayground_EquippedGridSlot : public UPlayground_GridSlot
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPlayground_EquippedSlottedItem* OnItemEquipped(UPlayground_InventoryItem* Item, const FGameplayTag& EquipmentTag, float TileSize);
	void SetEquippedSlottedItem(UPlayground_EquippedSlottedItem* Item) { EquippedSlottedItem = Item; }

	FEquippedGridSlotClicked EquippedGridSlotClicked;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "GameItems.Equipment"))
	FGameplayTag EquipmentTypeTag;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UImage> Image_GrayedOutIcon;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlayground_EquippedSlottedItem> EquippedSlottedItemClass;

	UPROPERTY()
	TObjectPtr<UPlayground_EquippedSlottedItem> EquippedSlottedItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Root;
};
