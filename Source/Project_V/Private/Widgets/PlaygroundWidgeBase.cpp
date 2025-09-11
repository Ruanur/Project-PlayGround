// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/PlaygroundWidgeBase.h"
#include "Interfaces/PawnUIInterface.h"

void UPlaygroundWidgeBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UPlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent())
		{
			BP_OnOwningPlayerUIComponentInitialized(PlayerUIComponent);
		}
	}
}
