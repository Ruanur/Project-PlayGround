// Copyright 2025. Jinsol Co. All rights reserved


#include "Inventory/Playground_InvComponent.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/PlaygroundWidgeBase.h"
#include "PlaygroundDebugHelper.h"

// Sets default values for this component's properties
UPlayground_InvComponent::UPlayground_InvComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPlayground_InvComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		Debug::Print(TEXT("Close Inventory"));
		CloseInventory();
	}
	else
	{
		Debug::Print(TEXT("Open Inventory"));
		OpenInventory();
	}
}

// Called when the game starts
void UPlayground_InvComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ConstructInventory();
}

void UPlayground_InvComponent::ConstructInventory()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	OwningController = Cast<APlayerController>(OwnerPawn->GetController());
	checkf(OwningController.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner"));

	if (!OwningController->IsLocalController()) return;
	if (!InventoryMenuClass)
	{
		Debug::Print(TEXT("InventoryMenuClass is not Set"));
		return;
	}

	InventoryMenu = CreateWidget<UPlaygroundWidgeBase>(OwningController.Get(), InventoryMenuClass);
	if (InventoryMenu)
	{
		InventoryMenu->AddToViewport();
		Debug::Print(TEXT("Inventory Menu Created"));
	}

	CloseInventory();
}

void UPlayground_InvComponent::OpenInventory()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UPlayground_InvComponent::CloseInventory()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);

}



