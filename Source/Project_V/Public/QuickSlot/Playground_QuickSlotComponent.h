// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Playground_QuickSlotTypes.h"

#include "Playground_QuickSlotComponent.generated.h"

class UPlayground_InventoryComponent;
class UPlayground_InventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickSlotsChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_V_API UPlayground_QuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayground_QuickSlotComponent();

	// UI가 슬롯 변경을 감지해서 갱신할 때 쓰는 이벤트
	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotsChanged OnQuickSlotsChanged;

	// 슬롯 개수 (Default: 4)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NumSlots = 4;

	// UI/블루프린트에서 호출, 슬롯에 아이템 할당
	UFUNCTION(BlueprintCallable)
	void AssignSlot(int32 SlotIndex, UPlayground_InventoryItem* Item);

	// UI/블루프린트에서 호출, 슬롯 사용
	UFUNCTION(BlueprintCallable)
	void UseSlot(int32 SlotIndex);

	// 슬롯 비우기
	UFUNCTION(BlueprintCallable)
	void ClearSlot(int32 SlotIndex);

	// QuickSlot Save & Load
	void BuildSaveData(TArray<FPlayground_QuickSlotRef>& Out) const;
	void ApplySaveData(const TArray<FPlayground_QuickSlotRef>& In);

	// UI 조회
	UFUNCTION(BlueprintCallable)
	const TArray<FPlayground_QuickSlotRef>& GetSlots() const { return Slots; }

protected:
	virtual void BeginPlay() override;

private:
	// 서버가 소유하는 퀵슬롯 (클라이언트에 복제됨)
	UPROPERTY(ReplicatedUsing = OnRep_Slots)
	TArray<FPlayground_QuickSlotRef> Slots;

	UFUNCTION()
	void OnRep_Slots();

	UPlayground_InventoryComponent* GetInventory() const;

	// Server RPC : 슬롯 할당
	UFUNCTION(Server, Reliable)
	void Server_AssignSlot(int32 SlotIndex, FGuid InstanceID, FName ItemID);

	// Server RPC: 슬롯 사용(소비)
	UFUNCTION(Server, Reliable)
	void Server_UseSlot(int32 SlotIndex);

	UFUNCTION(Server, Reliable)
	void Server_ClearSlot(int32 SlotIndex);

	bool PG_IsValidSlotIndex(int32 SlotIndex) const;
	void EnsureSlotsSized();

public:	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

		
};
