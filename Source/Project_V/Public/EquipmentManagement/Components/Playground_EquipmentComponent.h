// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Playground_EquipmentComponent.generated.h"

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
	void OnItemUnequipped(UPlayground_InventoryItem* EquippedItem);

	void InitInventoryComponent();
};
