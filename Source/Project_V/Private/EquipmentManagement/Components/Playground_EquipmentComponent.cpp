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

void UPlayground_EquipmentComponent::SetOwningSkeletalMesh(USkeletalMeshComponent* OwningMesh)
{
	OwningSkeletalMesh = OwningMesh;
}

void UPlayground_EquipmentComponent::InitializeOwner(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		OwningPlayerController = PlayerController;
	}
	InitInventoryComponent();
}

// Called when the game starts
void UPlayground_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerController();
}

void UPlayground_EquipmentComponent::InitPlayerController()
{
	if (OwningPlayerController = Cast<APlayerController>(GetOwner()); OwningPlayerController.IsValid())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()); IsValid(OwnerCharacter))
		{
			OwningSkeletalMesh = OwnerCharacter->GetMesh();
			OnPossessedPawnChange(nullptr, OwnerCharacter);
		}
		else
		{
			OwningPlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChange);
		}
	}
}

void UPlayground_EquipmentComponent::OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(NewPawn); IsValid(OwnerCharacter))
	{
		OwningSkeletalMesh = OwnerCharacter->GetMesh();
	}
	InitInventoryComponent();
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

	// nullptr Defense code
	if (!IsValid(SpawnedEquipActor))
	{
		return nullptr;
	}

	SpawnedEquipActor->SetEquipmentType(EquipmentFragment->GetEquipmentType());
	SpawnedEquipActor->SetOwner(GetOwner());
	EquipmentFragment->SetEquippedActor(SpawnedEquipActor);
	return SpawnedEquipActor;
}

APlayground_EquipActor* UPlayground_EquipmentComponent::FindEquippedActor(const FGameplayTag& EquipmentTypeTag)
{
	auto FoundActor = EquippedActors.FindByPredicate([&EquipmentTypeTag](const APlayground_EquipActor* EquippedActor)
		{
			return IsValid(EquippedActor) && EquippedActor->GetEquipmentType().MatchesTagExact(EquipmentTypeTag);
		});

	return FoundActor ? *FoundActor : nullptr;
}

void UPlayground_EquipmentComponent::RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag)
{
	if (APlayground_EquipActor* EquippedActor = FindEquippedActor(EquipmentTypeTag); IsValid(EquippedActor))
	{
		EquippedActors.Remove(EquippedActor);
		EquippedActor->Destroy();
	}
}

void UPlayground_EquipmentComponent::OnItemEquipped(UPlayground_InventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FPlayground_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>();
	if (!EquipmentFragment) return;

	float RarityMultiplier = 1.f;
	if (const FPlayground_ItemRarity* RarityFragment = ItemManifest.GetFragmentOfType<FPlayground_ItemRarity>())
	{
		RarityMultiplier = RarityFragment->GetStatMultiplier();
	}

	if (!bIsProxy)
	{
		EquipmentFragment->OnEquip(OwningPlayerController.Get(), RarityMultiplier);
	}
	

	if (!OwningSkeletalMesh.IsValid()) return;
	APlayground_EquipActor* SpawnedEquipActor = SpawnEquippedActor(EquipmentFragment, ItemManifest, OwningSkeletalMesh.Get());

	if (IsValid(SpawnedEquipActor))
	{
		EquippedActors.Add(SpawnedEquipActor);
	}

}

void UPlayground_EquipmentComponent::OnItemUnequipped(UPlayground_InventoryItem* UnequippedItem)
{
	if (!IsValid(UnequippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FPlayground_ItemManifest& ItemManifest = UnequippedItem->GetItemManifestMutable();
	FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>();
	if (!EquipmentFragment) return;

	if (!bIsProxy)
	{
		EquipmentFragment->OnUnequip(OwningPlayerController.Get());
	}

	RemoveEquippedActor(EquipmentFragment->GetEquipmentType());
}



