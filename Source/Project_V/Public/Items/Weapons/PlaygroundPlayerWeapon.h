// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/PlaygroundWeaponBase.h"
#include "PlayergroundTypes/PlaygroundStructTypes.h"
#include "PlaygroundPlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API APlaygroundPlayerWeapon : public APlaygroundWeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FPlaygroundPlayerWeaponData PlayerWeaponData;
};
