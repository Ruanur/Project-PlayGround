// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Playground_Leaf.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "Playground_RarityBorderLeaf.generated.h"

/**
 * 
 */

class UImage;
class UTexture2D;

UCLASS()
class PROJECT_V_API UPlayground_RarityBorderLeaf : public UPlayground_Leaf
{
	GENERATED_BODY()
	
public:
	// 테두리 색
	void SetBorderByRarity(EPlaygroundRarity InRarity) const;

private:
	FLinearColor GetRarityColor(EPlaygroundRarity InRarity) const;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Border_Image;
};
