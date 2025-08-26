// Copyright 2025. Jinsol Co. All rights reserved

#pragma once
#include "PlaygroundStructTypes.generated.h"

class UPlaygroundPlayerLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FPlaygroundPlayerWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlaygroundPlayerLinkedAnimLayer> WeaponAnimLayerToLink;
};
