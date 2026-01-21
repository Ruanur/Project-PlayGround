// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Composite/Playground_LeafText.h"
#include "Components/TextBlock.h"

void UPlayground_LeafText::PG_SetText(const FText& Text) const
{
	Text_LeafText->SetText(Text);
}

void UPlayground_LeafText::NativePreConstruct()
{
	Super::NativePreConstruct();

	FSlateFontInfo FontInfo = Text_LeafText->GetFont();
	FontInfo.Size = FontSize;


	Text_LeafText->SetFont(FontInfo);
}
