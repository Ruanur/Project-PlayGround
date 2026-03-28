// Copyright 2025. Jinsol Co. All rights reserved


#include "QuickSlot/Playground_QuickSlotComponent.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "GameFramework/Actor.h"
#include "SaveGame/PlaygroundSaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "PlaygroundDebugHelper.h"

UPlayground_QuickSlotComponent::UPlayground_QuickSlotComponent()
{
	SetIsReplicatedByDefault(true);
}


void UPlayground_QuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("[QuickSlotComp] BeginPlay Owner=%s"), *GetOwner()->GetName());
	EnsureSlotsSized();
	
	// 읽어서 Pending에 저장
	ReadQuickSlotFromSaveToPending();

	if (UPlayground_InventoryComponent* Inv = GetInventory())
	{
		Inv->OnInventoryRestored.AddUObject(this, &ThisClass::HandleInventoryRestored);
	}
}

void UPlayground_QuickSlotComponent::OnRep_Slots()
{
	// 클라이언트에서 Slots 배열이 갱신되면 UI 갱신 트리거
	OnQuickSlotsChanged.Broadcast();
}

UPlayground_InventoryComponent* UPlayground_QuickSlotComponent::GetInventory() const
{
	return GetOwner() ? GetOwner()->FindComponentByClass<UPlayground_InventoryComponent>() : nullptr;
}

bool UPlayground_QuickSlotComponent::PG_IsValidSlotIndex(int32 SlotIndex) const
{
	return SlotIndex >= 0 && SlotIndex < NumSlots;
}

void UPlayground_QuickSlotComponent::EnsureSlotsSized()
{
	if (Slots.Num() != NumSlots)
	{
		Slots.SetNum(NumSlots);
		for (int32 i = 0; i < NumSlots; ++i)
		{
			Slots[i].SlotIndex = i;
		}
	}
}

void UPlayground_QuickSlotComponent::ReadQuickSlotFromSaveToPending()
{
	PendingLoadedSlots.Reset();

	if (GetOwner() && !GetOwner()->HasAuthority()) return;
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex)) return;

	UPlaygroundSaveGame* SG = Cast<UPlaygroundSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));
	if (!SG) return;

	PendingLoadedSlots = SG->SavedQuickSlots;
	UE_LOG(LogTemp, Warning, TEXT("[QuickSlotComp] PendingLoadedSlots=%d"), PendingLoadedSlots.Num());

}

void UPlayground_QuickSlotComponent::ApplyPendingIfReady()
{
	if (bAppliedLoadedSlots) return;
	if (GetOwner() && !GetOwner()->HasAuthority()) return;

	// 인벤토리 아직 준비 안됐으면 리턴 (후에 호출 가능)
	UPlayground_InventoryComponent* Inv = GetInventory();
	if (!Inv) return;

	// Apply
	ApplySaveData(PendingLoadedSlots);
	bAppliedLoadedSlots = true;

	UE_LOG(LogTemp, Warning, TEXT("[QuickSlotComp] Apply Pending -> Done"));
}

void UPlayground_QuickSlotComponent::HandleInventoryRestored()
{
	ApplyPendingIfReady();
}

void UPlayground_QuickSlotComponent::SaveQuickSlot()
{
	if (GetOwner() && !GetOwner()->HasAuthority()) return;

	UPlaygroundSaveGame* SG = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		SG = Cast<UPlaygroundSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));
	}
	else
	{
		SG = Cast<UPlaygroundSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlaygroundSaveGame::StaticClass()));
	}

	if (!SG) return;

	BuildSaveData(SG->SavedQuickSlots);
	UGameplayStatics::SaveGameToSlot(SG, SaveSlotName, SaveUserIndex);
	Debug::Print(TEXT("[QuickSlot Comp] Save Quick Slot"));
}

void UPlayground_QuickSlotComponent::LoadQuickSlot()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex)) return;

	UPlaygroundSaveGame* SG = Cast<UPlaygroundSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));
	if (!SG) return;

	ApplySaveData(SG->SavedQuickSlots);
	Debug::Print(TEXT("[QuickSlot Comp] Load Quick Slot"));
}

void UPlayground_QuickSlotComponent::AssignSlot(int32 SlotIndex, UPlayground_InventoryItem* Item)
{
	if (!IsValid(Item) || !PG_IsValidSlotIndex(SlotIndex)) return;

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		Server_AssignSlot_Implementation(SlotIndex, Item->GetInstancedID(), Item->GetItemID());
	}
	else
	{
		// 클라이언트 -> 서버 요청
		Server_AssignSlot(SlotIndex, Item->GetInstancedID(), Item->GetItemID());
	}

	
}

void UPlayground_QuickSlotComponent::Server_AssignSlot_Implementation(int32 SlotIndex, FGuid InstanceID, FName ItemID)
{
	if (!PG_IsValidSlotIndex(SlotIndex)) return;

	UPlayground_InventoryComponent* Inv = GetInventory();
	if (!Inv)
	{
		Debug::Print(TEXT("[QuickSlot Comp] No Inventory"));
		return;
	}


	// 실제 아이템 존재하는지 검증 (치트 방지)
	UPlayground_InventoryItem* Found = Inv->PG_FindItemByInstanceID(InstanceID);
	if (!IsValid(Found))
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot Comp] Item not found by GUID=%s"), *InstanceID.ToString());
		return;
	}

	// 소비 아이템만 퀵슬롯 등록
	if (!Found->IsConsumable())
	{
		Debug::Print(TEXT("[QuickSlot Comp] Not Consumable"));
		return;
	}

	EnsureSlotsSized();

	Slots[SlotIndex].SlotIndex = SlotIndex;
	Slots[SlotIndex].InstanceID = InstanceID;
	Slots[SlotIndex].ItemID = ItemID;

	OnQuickSlotsChanged.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Assigned Slot=%d ItemID=%s GUID=%s"),
		SlotIndex,
		*Slots[SlotIndex].ItemID.ToString(),
		*Slots[SlotIndex].InstanceID.ToString());

	SaveQuickSlot();
}

void UPlayground_QuickSlotComponent::UseSlot(int32 SlotIndex)
{
	if (!PG_IsValidSlotIndex(SlotIndex)) return;

	Server_UseSlot_Implementation(SlotIndex); // 서버일 시 바로 처리 (현재 프로젝트는 싱글 플레이 중심, 추후 확장 가능성 열어둠)

}

void UPlayground_QuickSlotComponent::Server_UseSlot_Implementation(int32 SlotIndex)
{
	if (!PG_IsValidSlotIndex(SlotIndex)) return;

	UPlayground_InventoryComponent* Inv = GetInventory();
	if (!Inv) return;

	EnsureSlotsSized();

	const FGuid TargetID = Slots[SlotIndex].InstanceID;
	if (!TargetID.IsValid())
	{
		return;
	}

	UPlayground_InventoryItem* Item = Inv->PG_FindItemByInstanceID(TargetID);

	// 아이템이 사라지면 슬롯 비움
	if (!IsValid(Item))
	{
		Slots[SlotIndex].InstanceID.Invalidate();
		Slots[SlotIndex].ItemID = NAME_None;
		OnQuickSlotsChanged.Broadcast();
		return;
	}

	// 소비 아이템 아닐 시 사용 X
	if (!Item->IsConsumable())
	{
		return;
	}

	// 실제 소비는 인벤토리 컴포넌트에서 수행
	Inv->Server_ConsumeItem(Item);

	// 소비 결과 아이템이 사라졌으면 슬롯 자동 비움
	UPlayground_InventoryItem* After = Inv->PG_FindItemByInstanceID(TargetID);
	if (!IsValid(After))
	{
		Slots[SlotIndex].InstanceID.Invalidate();
		Slots[SlotIndex].ItemID = NAME_None;
		OnQuickSlotsChanged.Broadcast();
	}
	SaveQuickSlot();
}

void UPlayground_QuickSlotComponent::ClearSlot(int32 SlotIndex)
{
	if (!PG_IsValidSlotIndex(SlotIndex)) return;

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		Server_ClearSlot_Implementation(SlotIndex);
	}
	else
	{
		Server_ClearSlot(SlotIndex);
	}
}


void UPlayground_QuickSlotComponent::Server_ClearSlot_Implementation(int32 SlotIndex)
{
	if (!PG_IsValidSlotIndex(SlotIndex)) return;

	EnsureSlotsSized();

	Slots[SlotIndex].InstanceID.Invalidate();
	Slots[SlotIndex].ItemID = NAME_None;

	OnQuickSlotsChanged.Broadcast();
	SaveQuickSlot();
}

void UPlayground_QuickSlotComponent::BuildSaveData(TArray<FPlayground_QuickSlotRef>& Out) const
{
	Out = Slots;
}

void UPlayground_QuickSlotComponent::ApplySaveData(const TArray<FPlayground_QuickSlotRef>& In)
{
	EnsureSlotsSized();

	for (const FPlayground_QuickSlotRef& Ref : In)
	{
		if (!PG_IsValidSlotIndex(Ref.SlotIndex)) continue;
		Slots[Ref.SlotIndex] = Ref;
	}

	OnQuickSlotsChanged.Broadcast();
}


void UPlayground_QuickSlotComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayground_QuickSlotComponent, Slots);
}
