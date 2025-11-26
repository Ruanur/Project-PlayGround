// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Playground_GridSlot.generated.h"


class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_GridSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetTileIndex(int32 Index) { TileIndex = Index; }
	int32 GetTileIndex() const { return TileIndex; }
private:
	int32 TileIndex;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;
};
