// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Playground_InventoryComponent.generated.h"

class UPlaygroundWidgeBase;
class UPlayground_InventoryItem;
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UPlayground_InventoryItem*, Item);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECT_V_API UPlayground_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayground_InventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventoryMenu();

	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<APlayerController> OwningController;

	void ConstructInventory();

	UPROPERTY()
	TObjectPtr<UUserWidget> InventoryMenu;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UUserWidget> InventoryMenuClass;

	bool bInventoryMenuOpen;
	void OpenInventory();
	void CloseInventory();
};
