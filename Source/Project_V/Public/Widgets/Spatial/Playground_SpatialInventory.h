// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/PlaygroundWidgeBase.h"
#include "Playground_SpatialInventory.generated.h"

class UPlayground_InventoryGrid;
class UWidgetSwitcher;
class UButton;

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_SpatialInventory : public UPlaygroundWidgeBase
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
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

	UFUNCTION()
	void PG_ShowEquippables();

	UFUNCTION()
	void PG_ShowConsumables();

	UFUNCTION()
	void PG_ShowCraftables();
	void DisableButton(UButton* Button);

	void SetActiveGrid(UPlayground_InventoryGrid* Grid, UButton* Button);
	
};
