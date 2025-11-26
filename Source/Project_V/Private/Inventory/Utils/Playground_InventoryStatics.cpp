// Copyright 2025. Jinsol Co. All rights reserved


#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Inventory/Playground_InventoryComponent.h"

UPlayground_InventoryComponent* UPlayground_InventoryStatics::PG_GetInventoryComponent(const APlayerController* PlayerController)
{
    if (!IsValid(PlayerController)) return nullptr;

    APawn* Pawn = PlayerController->GetPawn();
    if (!IsValid(Pawn)) return nullptr;

    return Pawn->FindComponentByClass<UPlayground_InventoryComponent>();
    //UPlayground_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UPlayground_InventoryComponent>();
    //return InventoryComponent;
}
