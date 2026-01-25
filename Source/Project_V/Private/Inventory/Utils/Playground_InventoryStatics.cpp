// Copyright 2025. Jinsol Co. All rights reserved


#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Inventory/"
#include "Types/Playground_GridTypes.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "Widgets/PlaygroundWidgeBase.h"

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

void UPlayground_InventoryStatics::ItemHovered(APlayerController* PC, UPlayground_InventoryItem* Item)
{
    UPlayground_InventoryComponent* IC = PG_GetInventoryComponent(PC);
    if (!IsValid(IC)) return;

    UPlaygroundWidgeBase* InventoryBase = IC->GetInventoryMenu();
    if (!IsValid(InventoryBase)) return;

    if (InventoryBase->HasHoverItem()) return;

    InventoryBase->OnItemHovered(Item);
}


void UPlayground_InventoryStatics::ItemUnhovered(APlayerController* PC)
{
    UPlayground_InventoryComponent* IC = PG_GetInventoryComponent(PC);
    if (!IsValid(IC)) return;

    UPlaygroundWidgeBase* InventoryBase = IC->GetInventoryMenu();
    if (!IsValid(InventoryBase)) return;

    InventoryBase->OnItemUnHovered();
}

UPlayground_HoverItem* UPlayground_InventoryStatics::GetHoverItem(APlayerController* PC)
{
    UPlayground_InventoryComponent* IC = PG_GetInventoryComponent(PC);
    if (!IsValid(IC)) return nullptr;

    UPlaygroundWidgeBase* InventoryBase = IC->GetInventoryMenu();
    if (!IsValid(InventoryBase)) return nullptr;

    return InventoryBase->GetHoverItem();
}
