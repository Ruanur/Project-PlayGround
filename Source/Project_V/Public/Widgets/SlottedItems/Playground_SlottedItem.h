// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Playground_SlottedItem.generated.h"

class UImage;
class UPlayground_InventoryItem;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSlottedItemClicked, int32, GridIndex, const FPointerEvent&, MouseEvent);
/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_SlottedItem : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	bool IsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }
	UImage* GetImageIcon() const { return Image_Icon; }
	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetInventoryItem(UPlayground_InventoryItem* Item);
	UPlayground_InventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }
	void SetImageBrush(const FSlateBrush& Brush) const;
	void PG_UpdateStackCount(int32 StackCount);

	FSlottedItemClicked OnSlottedItemClicked;
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	int32 GridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UPlayground_InventoryItem> InventoryItem;
	bool bIsStackable{ false };
};
