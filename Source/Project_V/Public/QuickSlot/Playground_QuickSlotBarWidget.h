// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Playground_QuickSlotBarWidget.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;
class UPlayground_QuickSlotComponent;
class UPlayground_InventoryComponent;
struct FPlayground_SlotAvailabilityResult;

UCLASS()
class PROJECT_V_API UPlayground_QuickSlotBarWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SlotIcon0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SlotStack0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SlotIcon1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SlotStack1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SlotIcon2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SlotStack2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SlotIcon3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SlotStack3;

	TWeakObjectPtr<UPlayground_QuickSlotComponent> QS;
	TWeakObjectPtr<UPlayground_InventoryComponent> Inv;

	UFUNCTION()
	void PG_RefreshUI();

	UFUNCTION()
	void HandleQuickSlotsChanged();

	UFUNCTION()
	void HandleItemAdded(class UPlayground_InventoryItem* Item);

	UFUNCTION()
	void HandleStackChanged(const FPlayground_SlotAvailabilityResult& Result);

	void CacheComponents();
	void SetSlotEmpty(int32 SlotIndex);
	void SetSlotFromItem(int32 SlotIndex, class UPlayground_InventoryItem* Item, int32 TotalCount);
};
