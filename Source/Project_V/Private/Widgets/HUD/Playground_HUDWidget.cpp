// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/HUD/Playground_HUDWidget.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Widgets/HUD/Playground_InfoMessage.h"

#include "PlaygroundDebugHelper.h"

void UPlayground_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;

	Debug::Print(TEXT("Called?"));

	UPlayground_InventoryComponent* InventoryComponent = UPlayground_InventoryStatics::PG_GetInventoryComponent(PC);
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->NoRoomInInventory.AddDynamic(this, &ThisClass::PG_OnNoRoom);
		Debug::Print(TEXT("Vaild InvComp"));
	}
}

void UPlayground_HUDWidget::PG_OnNoRoom()
{
	if (!IsValid(InfoMessage))
	{
		Debug::Print(TEXT("InfoMessage is Null"));
		return;
	}
	InfoMessage->PG_SetMessage(InfoMessage->NoRoomMessage);
}
