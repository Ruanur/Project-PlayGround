// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Composite/Playground_LeafImage.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

void UPlayground_LeafImage::SetImage(UTexture2D* Texture) const
{
	Image_Icon->SetBrushFromTexture(Texture);
}

void UPlayground_LeafImage::SetBoxSize(const FVector2D& Size) const
{
	SizeBox_Icon->SetWidthOverride(Size.X);
	SizeBox_Icon->SetWidthOverride(Size.Y);
}

void UPlayground_LeafImage::SetImageSize(const FVector2D& Size) const
{
	Image_Icon->SetDesiredSizeOverride(Size);
}

FVector2D UPlayground_LeafImage::GetImageSize() const
{
	return Image_Icon->GetDesiredSize();
}
