// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Playground_QuickSlotWidget.generated.h"

/**
 * 
 */
class UButton;
class USizeBox;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnQuickSlotPicked, int32, SlotIndex);

UCLASS()
class PROJECT_V_API UPlayground_QuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	FOnQuickSlotPicked OnPicked;

	FVector2D PG_GetBoxSize() const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Slot1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Slot2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Slot3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Slot4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UFUNCTION()
	void PG_Slot1Clicked();

	UFUNCTION()
	void PG_Slot2Clicked();

	UFUNCTION()
	void PG_Slot3Clicked();

	UFUNCTION()
	void PG_Slot4Clicked();
};
