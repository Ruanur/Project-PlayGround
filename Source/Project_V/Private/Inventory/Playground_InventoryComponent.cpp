// Copyright 2025. Jinsol Co. All rights reserved


#include "Inventory/Playground_InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/PlaygroundWidgeBase.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Items/Fragment/Playground_ItemFragment.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "PlaygroundFunctionLibrary.h"
#include "Widgets/GridSlots/Playground_GridSlot.h"
#include "Widgets/Spatial/Playground_InventoryGrid.h"
#include "Engine/BlueprintGeneratedClass.h"   

#include "PlaygroundDebugHelper.h"


// Sets default values for this component's properties
UPlayground_InventoryComponent::UPlayground_InventoryComponent() : InventoryList(this)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
	// ...
}

void UPlayground_InventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}


void UPlayground_InventoryComponent::TryAddItem(UPlayground_ItemComponent* ItemComponent)
{
	FPlayground_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	UPlayground_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	Result.Item = FoundItem;

	//인벤토리 공간 없을 때 호출, 인벤토리가 꽉 참!
	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast();
	}
	
	if (Result.Item.IsValid() && Result.bStackable)
	{
		// Add Stacks to an Item that already exisits in the Inventory, We only want to update the stack count
		// not create a new item of this type.
		OnStackChange.Broadcast(Result);
		Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0)
	{
		// This item type doesn't exist in the inventory. Create a new one and update all pertinent slots.
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0, Result.Remainder);
	}
}

void UPlayground_InventoryComponent::Server_AddNewItem_Implementation(UPlayground_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	UPlayground_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);

	NewItem->Initialize(ItemComponent->GetItemManifest());
	NewItem->SetTotalStackCount(StackCount);

	// ItemID 확인 - None 호출됨
	UE_LOG(LogTemp, Warning, TEXT("Picked Item ID: %s"),
		*NewItem->GetItemID().ToString());

	// GUID 정상 작동.
	UE_LOG(LogTemp, Warning, TEXT("GUID: %s"),
		*NewItem->GetInstancedID().ToString());

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(NewItem);
	}

	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FPlayground_StackableFragment* StackableFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FPlayground_StackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}



	OnInventoryDataChanged.Broadcast();
}

void UPlayground_InventoryComponent::Server_AddStacksToItem_Implementation(UPlayground_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	const FGameplayTag& ItemType = IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	UPlayground_InventoryItem* Item = InventoryList.FindFirstItemByType(ItemType);
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);

	// TODO : Destroy the item if the Remainder is zero
	// Otherwise, update the stack count for the item pickup
	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FPlayground_StackableFragment* StackableFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FPlayground_StackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UPlayground_InventoryComponent::Server_DropItem_Implementation(UPlayground_InventoryItem* Item, int32 StackCount)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - StackCount;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	SpawnDroppedItem(Item, StackCount);
}

void UPlayground_InventoryComponent::SpawnDroppedItem(UPlayground_InventoryItem* Item, int32 StackCount)
{
	// TODO: Spawn the dropped item in the level
	const APawn* OwningPawn = OwningController->GetPawn();
	FVector RotatedForward = OwningPawn->GetActorForwardVector();


	// Drops Position
	RotatedForward = RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), FVector::UpVector);
	FVector SpawnLocation = OwningPawn->GetActorLocation() + RotatedForward * FMath::FRandRange(DropSpawnDistanceMin, DropSpawnDistanceMax);
	SpawnLocation.Z -= RelativeSpawnElevation;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	// TODO : Have the Item Manifest spawn the Pickup Actor
	FPlayground_ItemManifest ItemManifest = Item->GetItemManifestMutable();

	if (FPlayground_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_StackableFragment>())
	{
		StackableFragment->SetStackCount(StackCount);
	}
	ItemManifest.PG_SpawnPickupActor(this, SpawnLocation, SpawnRotation);
}

void UPlayground_InventoryComponent::Server_ConsumeItem_Implementation(UPlayground_InventoryItem* Item)
{
	if (!Item || !OwningController.IsValid())
	{
		return;
	}

	APawn* Pawn = OwningController->GetPawn();
	if (!Pawn) return;

	UPlaygroundAbilitySystemComponent* AbilitySystemComponent =
		Pawn->FindComponentByClass<UPlaygroundAbilitySystemComponent>();

	if (!AbilitySystemComponent) return;

	FPlayground_ConsumableFragment* ConsumableFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FPlayground_ConsumableFragment>();
	if (!ConsumableFragment) return;

	constexpr int32 ApplyLevel = 1;
	ConsumableFragment->OnConsume(
		OwningController.Get(),
		AbilitySystemComponent,
		ApplyLevel
	);


	const int32 NewStackCount = Item->GetTotalStackCount() - 1;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	// TODO: Get the Consumable fragment add call Consume()
	// (Actually create the Consumable Fragment)
	//if (FPlayground_ConsumableFragment* ConsumableFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FPlayground_ConsumableFragment>())
	//{
	//	ConsumableFragment->OnConsume(OwningController.Get());
	//}
}

void UPlayground_InventoryComponent::Server_EquipSlotClicked_Implementation(UPlayground_InventoryItem* ItemToEquip, UPlayground_InventoryItem* ItemToUnequip)
{	
	Multicast_EquipSlotClicked(ItemToEquip, ItemToUnequip);
}

void UPlayground_InventoryComponent::Multicast_EquipSlotClicked_Implementation(UPlayground_InventoryItem* ItemToEquip, UPlayground_InventoryItem* ItemToUnequip)
{
	// Equipment Component will listen to these delegates
	OnItemEquipped.Broadcast(ItemToEquip);
	OnItemUnequipped.Broadcast(ItemToUnequip);
}

void UPlayground_InventoryComponent::ToggleInventoryMenu()
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

	OnInventoryMenuToggled.Broadcast(bInventoryMenuOpen);
}

void UPlayground_InventoryComponent::AddRepSubObject(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

// Called when the game starts
void UPlayground_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ConstructInventory();
}

void UPlayground_InventoryComponent::ConstructInventory()
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

void UPlayground_InventoryComponent::OpenInventory()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UPlayground_InventoryComponent::CloseInventory()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);

	OnInventoryLoaded.Broadcast();
}







