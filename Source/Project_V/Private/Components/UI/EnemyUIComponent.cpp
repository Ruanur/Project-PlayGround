// Copyright 2025. Jinsol Co. All rights reserved


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/PlaygroundWidgeBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidget(UPlaygroundWidgeBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}

	for (UPlaygroundWidgeBase* DrawnWidget: EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}

	EnemyDrawnWidgets.Empty();

}
