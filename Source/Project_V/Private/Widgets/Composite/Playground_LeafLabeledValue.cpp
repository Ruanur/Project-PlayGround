// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Composite/Playground_LeafLabeledValue.h"
#include "Components/TextBlock.h"

void UPlayground_LeafLabeledValue::SetText_Label(const FText& Text, bool bCollapse) const
{
	if (bCollapse)
	{
		Text_Label->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	Text_Label->SetText(Text);
}

void UPlayground_LeafLabeledValue::SetText_Value(const FText& Text, bool bCollapse) const
{
	if (bCollapse)
	{
		Text_Value->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	Text_Value->SetText(Text);
}

void UPlayground_LeafLabeledValue::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!Text_Label || !Text_Value) return;

	FSlateFontInfo FontInfo_Label = Text_Label->GetFont();
	FontInfo_Label.Size = FontSize_Label;


	Text_Label->SetFont(FontInfo_Label);

	FSlateFontInfo FontInfo_Value = Text_Value->GetFont();
	FontInfo_Value.Size = FontSize_Value;


	Text_Value->SetFont(FontInfo_Value);
}
