// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Playground_GridTypes.h"
#include "Playground_InventoryGrid.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	EPlayground_ItemCategory GetItemCategory() const { return ItemCategory; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EPlayground_ItemCategory ItemCategory;
};
