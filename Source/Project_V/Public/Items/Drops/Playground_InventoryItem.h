// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/Drops/Manifest/Playground_ItemManifest.h"
#include "Playground_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_InventoryItem : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

	void SetItemManifest(const FPlayground_ItemManifest& Manifest);
	const FPlayground_ItemManifest& GetItemManifest() const { return ItemManifest.Get<FPlayground_ItemManifest>(); }
	FPlayground_ItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FPlayground_ItemManifest>(); }
	bool IsStackable() const;
	int32 GetTotalStackCount() const { return TotalStackCount; }
	void SetTotalStackCount(int32 Count) { TotalStackCount = Count; }

private:

	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.Playground_ItemManifest"), Replicated)
	FInstancedStruct ItemManifest;

	UPROPERTY(Replicated)
	int32 TotalStackCount{ 0 };
};

template <typename FragmentType>
const FragmentType* GetFragment(const UPlayground_InventoryItem* Item, const FGameplayTag& Tag)
{
	if (!IsValid(Item)) { return nullptr; }

	const FPlayground_ItemManifest& Manifest = Item->GetItemManifest();
	return Manifest.GetFragmentOfTypeWithTag<FragmentType>(Tag);
}