// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Playground_ItemComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECT_V_API UPlayground_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayground_ItemComponent();

	FString GetPickUpMessage() const { return PickupMessage; }

protected:

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};
