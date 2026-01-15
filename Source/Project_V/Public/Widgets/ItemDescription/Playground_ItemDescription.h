// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Playground_ItemDescription.generated.h"

/**
 * 
 */
class USizeBox;

UCLASS()
class PROJECT_V_API UPlayground_ItemDescription : public UUserWidget
{
	GENERATED_BODY()
public:

	FVector2D GetBoxSize() const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
