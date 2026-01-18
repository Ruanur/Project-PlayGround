// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Playground_Composite.h"
#include "Playground_ItemDescription.generated.h"

/**
 * 
 */
class USizeBox;

UCLASS()
class PROJECT_V_API UPlayground_ItemDescription : public UPlayground_Composite
{
	GENERATED_BODY()

public:

	FVector2D GetBoxSize() const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
