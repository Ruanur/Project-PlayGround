// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Playground_CompositeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_CompositeBase : public UUserWidget
{
	GENERATED_BODY()

public:
	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(const FGameplayTag& Tag) { FragmentTag = Tag; }
	virtual void PG_Collapse();
	void PG_Expand();

	using FuncType = TFunction<void(UPlayground_CompositeBase*)>;
	virtual void ApplyFunction(FuncType Function) {}


private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag;
	
};
