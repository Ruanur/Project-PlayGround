// Copyright 2025. Jinsol Co. All rights reserved


#include "QuickSlot/Playground_QuickSlotWidget.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"

#include "PlaygroundDebugHelper.h"

void UPlayground_QuickSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Slot1->OnClicked.AddDynamic(this, &ThisClass::PG_Slot1Clicked);
	Button_Slot2->OnClicked.AddDynamic(this, &ThisClass::PG_Slot2Clicked);
	Button_Slot3->OnClicked.AddDynamic(this, &ThisClass::PG_Slot3Clicked);
	Button_Slot4->OnClicked.AddDynamic(this, &ThisClass::PG_Slot4Clicked);
}

void UPlayground_QuickSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}

void UPlayground_QuickSlotWidget::PG_Slot1Clicked()
{
	Debug::Print(TEXT("[QuickSlot Widget] Slot 1 Clicked"));
	OnPicked.ExecuteIfBound(0);
	RemoveFromParent();
}

void UPlayground_QuickSlotWidget::PG_Slot2Clicked()
{
	Debug::Print(TEXT("[QuickSlot Widget] Slot 2 Clicked"));
	OnPicked.ExecuteIfBound(1);
	RemoveFromParent();
}

void UPlayground_QuickSlotWidget::PG_Slot3Clicked()
{
	Debug::Print(TEXT("[QuickSlot Widget] Slot 3 Clicked"));
	OnPicked.ExecuteIfBound(2);
	RemoveFromParent();
}

void UPlayground_QuickSlotWidget::PG_Slot4Clicked()
{
	Debug::Print(TEXT("[QuickSlot Widget] Slot 4 Clicked"));
	OnPicked.ExecuteIfBound(3);
	RemoveFromParent();
}

FVector2D UPlayground_QuickSlotWidget::PG_GetBoxSize() const
{
	return FVector2D(SizeBox_Root->GetWidthOverride(), SizeBox_Root->GetHeightOverride());
}