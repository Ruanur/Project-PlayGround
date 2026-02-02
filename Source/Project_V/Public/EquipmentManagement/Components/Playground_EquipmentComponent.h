// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	

protected:
	virtual void BeginPlay() override;

private:	

	TWeakObjectPtr<UPlayground_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<APlayerController> OwningPlayerController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;

	UFUNCTION()
	void OnItemEquipped(UPlayground_InventoryItem* EquippedItem);

	UFUNCTION()
	void OnItemUnequipped(UPlayground_InventoryItem* UnequippedItem);

	void InitPlayerController();
	void InitInventoryComponent();
	APlayground_EquipActor* SpawnEquippedActor(FPlayground_EquipmentFragment* EquipmentFragment, const FPlayground_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh);

	UPROPERTY()
	TArray<TObjectPtr<APlayground_EquipActor>> EquippedActors;

	APlayground_EquipActor* FindEquippedActor(const FGameplayTag& EquipmentTypeTag);
	void RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag);
	
	UFUNCTION()
	void OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn); 
};
