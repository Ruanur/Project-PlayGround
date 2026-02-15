// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/FastArray/Playground_FastArray.h"
#include "Inventory/Save/Playground_FInventorySlotInfo.h"
#include "Items/Drops/Manifest/Playground_ItemManifest.h"
#include "Playground_InventoryComponent.generated.h"

class UPlayground_ItemComponent;
class UPlayground_InventoryItem;
class UPlayground_InventoryGrid;
class UPlaygroundWidgeBase;
class UPlaygroundWidgeBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UPlayground_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FPlayground_SlotAvailabilityResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipStatusChanged, UPlayground_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryMenuToggled, bool, bOpen);
DECLARE_MULTICAST_DELEGATE(FOnInventoryDataChanged);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECT_V_API UPlayground_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayground_InventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Unable to find 'class', 'delegate', 'enum', or 'struct' with name 'UPlayground_ItemComponent
	//Çü º¯È¯, AActor -> UActorComponent
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UPlayground_ItemComponent* ItemComponent);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UPlayground_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UPlayground_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_DropItem(UPlayground_InventoryItem* Item, int32 StackCount);

	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(UPlayground_InventoryItem* Item);

	UFUNCTION(Server, Reliable)
	void Server_EquipSlotClicked(UPlayground_InventoryItem* ItemToEquip, UPlayground_InventoryItem* ItemToUnequip);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EquipSlotClicked(UPlayground_InventoryItem* ItemToEquip, UPlayground_InventoryItem* ItemToUnequip);

	void ToggleInventoryMenu();
	void AddRepSubObject(UObject* SubObj);
	void SpawnDroppedItem(UPlayground_InventoryItem* Item, int32 StackCount);
	UPlaygroundWidgeBase* GetInventoryMenu() const { return InventoryMenu; }

	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FNoRoomInInventory NoRoomInInventory;
	FStackChange OnStackChange;
	FItemEquipStatusChanged OnItemEquipped;
	FItemEquipStatusChanged OnItemUnequipped;
	FInventoryMenuToggled OnInventoryMenuToggled;
	FOnInventoryDataChanged OnInventoryDataChanged;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<APlayerController> OwningController;

	void ConstructInventory();

	UPROPERTY(Replicated)
	FPlayground_InventoryFastArray InventoryList;

	UPROPERTY()
	TObjectPtr<UPlaygroundWidgeBase> InventoryMenu;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlaygroundWidgeBase> InventoryMenuClass;

	UPROPERTY()
	TObjectPtr<UPlayground_InventoryGrid> InventoryGrid;

	bool bInventoryMenuOpen;
	void OpenInventory();
	void CloseInventory();

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMin = -85.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMax = 85.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMin = 10.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMax = 50.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float RelativeSpawnElevation = -70.f;


};
