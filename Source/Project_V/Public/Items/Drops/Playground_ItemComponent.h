// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Drops/Manifest/Playground_ItemManifest.h"
#include "Playground_ItemComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECT_V_API UPlayground_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayground_ItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitItemManifest(FPlayground_ItemManifest CopyOfManifest);

	FPlayground_ItemManifest GetItemManifest() const { return ItemManifest; }
	FPlayground_ItemManifest& GetItemManifestMutable() { return ItemManifest; }

	FString GetPickUpMessage() const { return PickupMessage; }
	void PickedUp();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnPickedUp();

private:
	UPROPERTY(EditAnywhere, Replicated, Category = "Inventory")
	FPlayground_ItemManifest ItemManifest;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};
