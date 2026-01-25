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

UCLASS()
class PROJECT_V_API UPlayground_EquippedGridSlot : public UPlayground_GridSlot
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	FEquippedGridSlotClicked EquippedGridSlotClicked;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag EquipmentTypeTag;
};
