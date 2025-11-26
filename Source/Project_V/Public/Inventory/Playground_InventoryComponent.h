// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Playground_InventoryComponent.generated.h"

class UPlayground_ItemComponent;
class UPlayground_InventoryItem;
class UPlaygroundWidgeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UPlayground_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECT_V_API UPlayground_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayground_InventoryComponent();

	//Unable to find 'class', 'delegate', 'enum', or 'struct' with name 'UPlayground_ItemComponent
	//Çü º¯È¯, AActor -> UActorComponent
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UPlayground_ItemComponent* ItemComponent);

	void ToggleInventoryMenu();

	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FNoRoomInInventory NoRoomInInventory;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<APlayerController> OwningController;

	void ConstructInventory();

	UPROPERTY()
	TObjectPtr<UPlaygroundWidgeBase> InventoryMenu;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlaygroundWidgeBase> InventoryMenuClass;

	bool bInventoryMenuOpen;
	void OpenInventory();
	void CloseInventory();
};
