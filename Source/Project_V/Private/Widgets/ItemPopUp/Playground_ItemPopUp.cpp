// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/ItemPopUp/Playground_ItemPopUp.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UPlayground_ItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Split->OnClicked.AddDynamic(this, &ThisClass::PG_SplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &ThisClass::PG_DropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &ThisClass::PG_ConsumeButtonClicked);
	Slider_Split->OnValueChanged.AddDynamic(this, &ThisClass::PG_SliderValueChanged);
}

void UPlayground_ItemPopUp::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
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
