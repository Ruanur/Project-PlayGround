// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Playground_GridSlot.generated.h"


class UImage;
class UPlayground_InventoryItem;

UENUM(BlueprintType)
enum class EPlayground_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};


UCLASS()
class PROJECT_V_API UPlayground_GridSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetTileIndex(int32 Index) { TileIndex = Index; }
	int32 GetTileIndex() const { return TileIndex; }
	EPlayground_GridSlotState GetGridSlotState() const { return GridSlotState; }
	TWeakObjectPtr<UPlayground_InventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(UPlayground_InventoryItem* Item);
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }
	int32 GetIndex() const { return TileIndex; }
	void SetIndex(int32 Index) { TileIndex = Index; }
	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }
	bool IsAvailable() const { return bAvailable; }
	void SetAvailable(bool bIsAvailable) { bAvailable = bIsAvailable; }

	void PG_SetOccupiedTexture();
	void PG_SetUnoccupiedTexture();
	void PG_SetSelectedTexture();
	void PG_SetGrayedOutTexture();

private:
	int32 TileIndex;
	int32 StackCount;
	int32 UpperLeftIndex{ INDEX_NONE };
	TWeakObjectPtr<UPlayground_InventoryItem> InventoryItem;
	bool bAvailable;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;

	EPlayground_GridSlotState GridSlotState;


};
