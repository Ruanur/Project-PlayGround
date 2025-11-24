// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Playground_InvComponent.generated.h"

class UPlaygroundWidgeBase;
class UUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECT_V_API UPlayground_InvComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayground_InvComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventoryMenu();

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
