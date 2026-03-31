// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/PlaygroundPickUpBase.h"
#include "Characters/PlaygroundPlayerCharacter.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "PlaygroundDropsBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API APlaygroundDropsBase : public APlaygroundPickUpBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Object Interacted"))
	void BP_ObjectInteracted(APlaygroundPlayerCharacter* InteractingPlayer);

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

#pragma region Loot_ItemManifest
public:
	UFUNCTION(BlueprintCallable, Category = "Drop")
	void InitializeDropFromRarity(EPlaygroundRarity InRarity);

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Drop")
	EPlaygroundRarity DroppedRarity = EPlaygroundRarity::Common;
#pragma endregion

};
