// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Playground_CompositeBase.h"
#include "Playground_Composite.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_Composite : public UPlayground_CompositeBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void ApplyFunction(FuncType Function) override;
	virtual void PG_Collapse() override;
	TArray<UPlayground_CompositeBase*> GetChildren() { return Children; }

private:
	UPROPERTY()
	TArray<TObjectPtr<UPlayground_CompositeBase>> Children;

};
