// Copyright 2025. Jinsol Co. All rights reserved


#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Types/Playground_GridTypes.h"
#include "Items/Drops/Playground_ItemComponent.h"

#include "PlaygroundDebugHelper.h"
UPlayground_InventoryComponent* UPlayground_InventoryStatics::PG_GetInventoryComponent(const APlayerController* PlayerController)
{
    if (!IsValid(PlayerController))
    {
        Debug::Print(TEXT("Called PG_GetInventoryComponent, Controller Is Valid"));
        return nullptr;
    }
    APawn* Pawn = PlayerController->GetPawn();
    if (!IsValid(Pawn))
    {
        Debug::Print(TEXT("Called PG_GetInventoryComponent, Pawn Is Valid"));
        return nullptr;
    }

    return Pawn->FindComponentByClass<UPlayground_InventoryComponent>();
    
    //UPlayground_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UPlayground_InventoryComponent>();
    //return InventoryComponent;
}

EPlayground_ItemCategory UPlayground_InventoryStatics::GetItemCategoryFromItemComp(UPlayground_ItemComponent* ItemComp)
{
    if (!IsValid(ItemComp)) return EPlayground_ItemCategory::None;
    return ItemComp->GetItemManifest().GetItemCategory();
}
