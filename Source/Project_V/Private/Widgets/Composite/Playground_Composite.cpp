// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/Composite/Playground_Composite.h"
#include "Blueprint/WidgetTree.h"

void UPlayground_Composite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
		{
			if (UPlayground_CompositeBase* Composite = Cast<UPlayground_CompositeBase>(Widget); IsValid(Composite))
			{
				Children.Add(Composite);
				Composite->PG_Collapse();
			}
		});
}

void UPlayground_Composite::ApplyFunction(FuncType Function)
{
	for (auto& Child : Children)
	{
		Child->ApplyFunction(Function);
	}

}

void UPlayground_Composite::PG_Collapse()
{
	for (auto& Child : Children)
	{
		Child->PG_Collapse();
	}
}
