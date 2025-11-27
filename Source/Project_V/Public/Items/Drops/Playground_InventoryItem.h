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


	void SetItemManifest(const FPlayground_ItemManifest& Manifest);
private:

	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.Playground_ItemManifest"), Replicated)
	FInstancedStruct ItemManifest;
};
