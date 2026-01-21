// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Playground_Leaf.h"
#include "Playground_LeafLabeledValue.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class PROJECT_V_API UPlayground_LeafLabeledValue : public UPlayground_Leaf
{
	GENERATED_BODY()

public:

	void SetText_Label(const FText& Text, bool bCollapse) const;
	void SetText_Value(const FText& Text, bool bCollapse) const;
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Label;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Value;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 FontSize_Label{ 12 };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 FontSize_Value{ 18 };
};
