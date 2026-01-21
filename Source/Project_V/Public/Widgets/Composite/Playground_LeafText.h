// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Playground_Leaf.h"
#include "Playground_LeafText.generated.h"

/**
 * 
 */

class UTextBlock;
UCLASS()
class PROJECT_V_API UPlayground_LeafText : public UPlayground_Leaf
{
	GENERATED_BODY()
	
public:
	void PG_SetText(const FText& Text) const;
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_LeafText;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 FontSize{12};
};
