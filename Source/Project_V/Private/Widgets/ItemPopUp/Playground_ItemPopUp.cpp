// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/ItemPopUp/Playground_ItemPopUp.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "QuickSlot/Playground_QuickSlotWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "PlaygroundDebugHelper.h"

void UPlayground_ItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Split->OnClicked.AddDynamic(this, &ThisClass::PG_SplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &ThisClass::PG_DropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &ThisClass::PG_ConsumeButtonClicked);
	Slider_Split->OnValueChanged.AddDynamic(this, &ThisClass::PG_SliderValueChanged);
	Button_AssignQuick->OnClicked.AddDynamic(this, &ThisClass::PG_AssignQuickButtonClicked);
}

void UPlayground_ItemPopUp::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (IsValid(QuickSlotSelectWidget))
	{
		return;
	}

	RemoveFromParent();
}

int32 UPlayground_ItemPopUp::PG_GetSplitAmount() const
{
	return FMath::Floor(Slider_Split->GetValue());
}

void UPlayground_ItemPopUp::PG_SplitButtonClicked()
{
	if (OnSplit.ExecuteIfBound(PG_GetSplitAmount(), GridIndex))
	{
		RemoveFromParent();
	}
}

void UPlayground_ItemPopUp::PG_DropButtonClicked()
{
	if (OnDrop.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UPlayground_ItemPopUp::PG_ConsumeButtonClicked()
{
	if (OnConsume.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UPlayground_ItemPopUp::PG_SliderValueChanged(float Value)
{
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}


void UPlayground_ItemPopUp::PG_CollapseSplitButton() const
{
	Button_Split->SetVisibility(ESlateVisibility::Collapsed);
	Slider_Split->SetVisibility(ESlateVisibility::Collapsed);
	Text_SplitAmount->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayground_ItemPopUp::PG_CollapseConsumeButton() const
{
	Button_Consume->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayground_ItemPopUp::PG_CollapseQuickAssignButton() const
{
	Button_AssignQuick->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayground_ItemPopUp::SetSliderParams(const float Max, const float Value) const
{
	Slider_Split->SetMaxValue(Max);
	Slider_Split->SetMinValue(1);
	Slider_Split->SetValue(Value);
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}


FVector2D UPlayground_ItemPopUp::PG_GetBoxSize() const
{
	return FVector2D(SizeBox_Root->GetWidthOverride(), SizeBox_Root->GetHeightOverride());
}

// 퀵슬롯 등록 버튼 클릭 -> 선택창 띄우기
void UPlayground_ItemPopUp::PG_AssignQuickButtonClicked()
{
	Debug::Print(TEXT("[PopUp] AssignQuick Clicked"));

	if (IsValid(QuickSlotSelectWidget))
	{
		Debug::Print(TEXT("[PopUp Close existing QuickSLotSelectWidget]"));
		QuickSlotSelectWidget->RemoveFromParent();
		QuickSlotSelectWidget = nullptr;
		return;
	}

	if (!QuickSlotSelectWidgetClass)
	{
		Debug::Print(TEXT("[PopUp] QuickSlotSelectWidgetClass is Null"));
		return;
	}

	QuickSlotSelectWidget = CreateWidget<UPlayground_QuickSlotWidget>(GetOwningPlayer(), QuickSlotSelectWidgetClass);
	if (!IsValid(QuickSlotSelectWidget)) return;

	QuickSlotSelectWidget->OnPicked.BindDynamic(this, &ThisClass::PG_OnQuickSlotPicked);

	if (OwningCanvasPanel)
	{
		OwningCanvasPanel->AddChild(QuickSlotSelectWidget);

		if (UCanvasPanelSlot* QSCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(QuickSlotSelectWidget))
		{
			QSCanvasSlot->SetAutoSize(true);
			QSCanvasSlot->SetAnchors(FAnchors(0.f, 0.f));
			QSCanvasSlot->SetAlignment(FVector2D(0.f, 0.f));
			QSCanvasSlot->SetZOrder(10000);

			// 팝업 자기 자신의 캔버스 위치 기준으로 옆에 띄우기
			FVector2D PopupPos(0.f, 0.f);
			if (UCanvasPanelSlot* PopupCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this))
			{
				PopupPos = PopupCanvasSlot->GetPosition();
			}

			// 팝업 오른쪽에 살짝 띄우기 (원하면 오프셋 조절)
			const FVector2D Offset(20.f, 0.f);
			QSCanvasSlot->SetPosition(PopupPos + Offset);
		}
		return;
	}
	//fallback : 캔버스 못찾으면 viewport로 띄움 (미설정시)
	QuickSlotSelectWidget->AddToViewport(9999);
}

void UPlayground_ItemPopUp::PG_OnQuickSlotPicked(int32 SlotIndex)
{
	OnAssignQuick.ExecuteIfBound(SlotIndex, GridIndex);

	if (IsValid(QuickSlotSelectWidget))
	{
		QuickSlotSelectWidget->RemoveFromParent();
		QuickSlotSelectWidget = nullptr;
	}

	RemoveFromParent();
}
