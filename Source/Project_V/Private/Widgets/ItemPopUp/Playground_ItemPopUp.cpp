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

void UPlayground_ItemPopUp::PG_SplitButtonClicked()
{

}

void UPlayground_ItemPopUp::PG_DropButtonClicked()
{

}

void UPlayground_ItemPopUp::PG_ConsumeButtonClicked()
{

}

void UPlayground_ItemPopUp::PG_SliderValueChanged(float Value)
{

}
