// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Playground_GridTypes.h"
#include "PlaygroundWidgeBase.generated.h"

class UPlayerUIComponent;
class UEnemyUIComponent;
class UPlayground_ItemComponent;
class UPlayground_InventoryItem;
class UPlayground_HoverItem;
/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundWidgeBase : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Hero UI Component Initialized"))
	void BP_OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* OwningPlayerUIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Enemy UI Component Initialized"))
	void BP_OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningEnemyUIComponent);

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);

	virtual FPlayground_SlotAvailabilityResult HasRoomForItem(UPlayground_ItemComponent* ItemComponent) const { return FPlayground_SlotAvailabilityResult(); }
	virtual void OnItemHovered(UPlayground_InventoryItem* Item) {}
	virtual void OnItemUnHovered() {}
	virtual bool HasHoverItem() const { return false; }
	virtual UPlayground_HoverItem* GetHoverItem() const { return nullptr; }
};
