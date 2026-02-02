// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Playground_EquipActor.generated.h"

UCLASS()
class PROJECT_V_API APlayground_EquipActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayground_EquipActor();
	FGameplayTag GetEquipmentType() const { return EquipmentType; }
	void SetEquipmentType(FGameplayTag Type) { EquipmentType = Type; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag EquipmentType;
};
