// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Playground_Leaf.h"
#include "Playground_LeafImage.generated.h"

/**
 * 
 */
class UImage;
class USizeBox;

UCLASS()
class PROJECT_V_API UPlayground_LeafImage : public UPlayground_Leaf
{
	GENERATED_BODY()

public:

	void SetImage(UTexture2D* Texture) const;
	void SetBoxSize(const FVector2D& Size) const;
	void SetImageSize(const FVector2D& Size) const;
	FVector2D GetImageSize() const;

private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Icon;
};
