// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/HUD/Playground_InfoMessage.h"
#include "Components/TextBlock.h"

#include "PlaygroundDebugHelper.h"
void UPlayground_InfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	MessageHide();
}

void UPlayground_InfoMessage::PG_SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);
	
	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(MessageTimer, [this]() 
		{
			MessageHide();
			bIsMessageActive = false;
		}, MessageLifetime, false
	);
}
