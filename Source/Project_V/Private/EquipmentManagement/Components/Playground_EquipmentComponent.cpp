// Copyright 2025. Jinsol Co. All rights reserved


#include "EquipmentManagement/Components/Playground_EquipmentComponent.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Inventory/Utils/Playground_InventoryStatics.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Items/Drops/Manifest/Playground_ItemManifest.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Items/Fragment/Playground_ItemFragment.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts
void UPlayground_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayerController = Cast<APlayerController>(GetOwner());
	if (OwningPlayerController.IsValid())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()); IsValid(OwnerCharacter))
		{
			OwningSkeletalMesh = OwnerCharacter->GetMesh();
		}
		InitInventoryComponent();
	}
}

void UPlayground_EquipmentComponent::InitInventoryComponent()
{
	InventoryComponent = UPlayground_InventoryStatics::PG_GetInventoryComponent(OwningPlayerController.Get());
	if (!InventoryComponent.IsValid()) return;

	if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this, &ThisClass::OnItemEquipped))
	{
		InventoryComponent->OnItemEquipped.AddDynamic(this, &ThisClass::OnItemEquipped);
	}

	if (!InventoryComponent->OnItemUnequipped.IsAlreadyBound(this, &ThisClass::OnItemUnequipped))
	{
		InventoryComponent->OnItemUnequipped.AddDynamic(this, &ThisClass::OnItemUnequipped);
	}
}

void UPlayground_EquipmentComponent::OnItemEquipped(UPlayground_InventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FPlayground_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>();
	if (!EquipmentFragment) return;

	EquipmentFragment->OnEquip(OwningPlayerController.Get());
}

void UPlayground_EquipmentComponent::OnItemUnequipped(UPlayground_InventoryItem* UnequippedItem)
{
	if (!IsValid(UnequippedItem)) return;
	if (OwningPlayerController->HasAuthority()) return;

	FPlayground_ItemManifest& ItemManifest = UnequippedItem->GetItemManifestMutable();
	FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>();
	if (!EquipmentFragment) return;

	EquipmentFragment->OnUnequip(OwningPlayerController.Get());
}



