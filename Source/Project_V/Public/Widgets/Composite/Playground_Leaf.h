// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Playground_CompositeBase.h"
#include "Playground_Leaf.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_Leaf : public UPlayground_CompositeBase
{
	GENERATED_BODY()
public:
	virtual void ApplyFunction(FuncType Function) override;
	
};
