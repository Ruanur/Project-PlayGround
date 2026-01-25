// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SlottedItems/Playground_SlottedItem.h"
#include "GameplayTagContainer.h"
#include "Playground_EquippedSlottedItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquippedSlottedItemClicked, class UPlayground_EquippedSlottedItem*, SlottedItem);

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_EquippedSlottedItem : public UPlayground_SlottedItem
{
	GENERATED_BODY()
	
public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetEquipmentTypeTag(const FGameplayTag& Tag) { EquipmentTypeTag = Tag; }
	FGameplayTag GetEquipmentTypeTag() const { return EquipmentTypeTag; }

	FEquippedSlottedItemClicked OnEquippedSlottedItemClicked;
private:

	UPROPERTY()
	FGameplayTag EquipmentTypeTag;

};
