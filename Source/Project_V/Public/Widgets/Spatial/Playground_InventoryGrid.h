// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Playground_GridTypes.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Playground_InventoryGrid.generated.h"


class UCanvasPanel;
class UPlayground_GridSlot;
class UPlayground_InventoryComponent;

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

	UFUNCTION()
	void AddItem(UPlayground_InventoryItem* Item);
private:
	TWeakObjectPtr<UPlayground_InventoryComponent> InventoryComponent;

	void ConstructGrid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EPlayground_ItemCategory ItemCategory;

	UPROPERTY()
	TArray<TObjectPtr<UPlayground_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlayground_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;

	bool MatchesCategory(const UPlayground_InventoryItem* Item) const;
};
