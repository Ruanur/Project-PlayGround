// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/ItemPopUp/Playground_ItemPopUp.h"

#include "Components/Button.h"
#include "Components/Slider.h"

void UPlayground_ItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Split->OnClicked.AddDynamic(this, &ThisClass::PG_SplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &ThisClass::PG_DropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &ThisClass::PG_ConsumeButtonClicked);
	Slider_Split->OnValueChanged.AddDynamic(this, &ThisClass::PG_SliderValueChanged);
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

}
