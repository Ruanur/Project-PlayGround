// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "Items/Drops/Manifest/Playground_ItemManifest.h"
#include "Playground_WorldDroppedItem.generated.h"

class UPlayground_InventoryItem;

UCLASS()
class PROJECT_V_API APlayground_WorldDroppedItem : public AActor
{
	GENERATED_BODY()

public:
	void InitializedDroppedItem(const FPlayground_ItemManifest& InManifest, EPlaygroundRarity InRarity);

	UFUNCTION(BlueprintPure, Category = "Loot")
	UPlayground_InventoryItem* GetInventoryItem() const { return InventoryItem; }

	UFUNCTION(BlueprintPure, Category = "Loot")
	EPlaygroundRarity GetDroppedRarity() const { return DroppedRarity; }


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot")
	TObjectPtr<UPlayground_InventoryItem> InventoryItem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot")
	EPlaygroundRarity DroppedRarity = EPlaygroundRarity::Common;
	

};
