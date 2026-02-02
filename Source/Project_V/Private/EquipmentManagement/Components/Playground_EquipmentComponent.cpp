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
#include "EquipmentManagement/EqiupActor/Playground_EquipActor.h"

#include "PlaygroundDebugHelper.h"

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

APlayground_EquipActor* UPlayground_EquipmentComponent::SpawnEquippedActor(FPlayground_EquipmentFragment* EquipmentFragment, const FPlayground_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh)
{
	APlayground_EquipActor* SpawnedEquipActor = EquipmentFragment->SpawnAttachedActor(AttachMesh);

	SpawnedEquipActor->SetEquipmentType(EquipmentFragment->GetEquipmentType());
	SpawnedEquipActor->SetOwner(GetOwner());
	EquipmentFragment->SetEquippedActor(SpawnedEquipActor);
	return SpawnedEquipActor;
}

void UPlayground_EquipmentComponent::OnItemEquipped(UPlayground_InventoryItem* EquippedItem)
{
	// Error : EquippedItem = Nullptr 값으로 호출됨
	if (!IsValid(EquippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FPlayground_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>();
	if (!EquipmentFragment) return;

	EquipmentFragment->OnEquip(OwningPlayerController.Get());


	if (!OwningSkeletalMesh.IsValid()) return;
	APlayground_EquipActor* SpawnedEquipActor = SpawnEquippedActor(EquipmentFragment, ItemManifest, OwningSkeletalMesh.Get());

	EquippedActors.Add(SpawnedEquipActor);

}

void UPlayground_EquipmentComponent::OnItemUnequipped(UPlayground_InventoryItem* UnequippedItem)
{
	if (!IsValid(UnequippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FPlayground_ItemManifest& ItemManifest = UnequippedItem->GetItemManifestMutable();
	FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>();
	if (!EquipmentFragment) return;

	EquipmentFragment->OnUnequip(OwningPlayerController.Get());
}



