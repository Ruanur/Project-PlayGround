// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Inventory/Save/Playground_FInventorySlotInfo.h"
#include "Playground_EquipmentComponent.generated.h"

struct FPlayground_EquipmentFragment;
struct FPlayground_ItemManifest;
struct FGameplayTag;
class APlayground_EquipActor;
class UPlayground_InventoryComponent;
class UPlayground_InventoryItem;
class APlayerController;
class USkeletalMeshComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECT_V_API UPlayground_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	void SetOwningSkeletalMesh(USkeletalMeshComponent* OwningMesh);
	void SetIsProxy(bool bProxy) { bIsProxy = bProxy; }
	void InitializeOwner(APlayerController* PlayerController);

protected:
	virtual void BeginPlay() override;

private:	
	void InitPlayerController();
	void InitInventoryComponent();

	UFUNCTION()
	void OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn);

	TWeakObjectPtr<UPlayground_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<APlayerController> OwningPlayerController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;

	UPROPERTY()
	TArray<TObjectPtr<APlayground_EquipActor>> EquippedActors;

	UFUNCTION()
	void OnItemEquipped(UPlayground_InventoryItem* EquippedItem);

	UFUNCTION()
	void OnItemUnequipped(UPlayground_InventoryItem* UnequippedItem);

	APlayground_EquipActor* SpawnEquippedActor(FPlayground_EquipmentFragment* EquipmentFragment, const FPlayground_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh);

	APlayground_EquipActor* FindEquippedActor(const FGameplayTag& EquipmentTypeTag);
	void RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag);

	bool bIsProxy{ false };

};
