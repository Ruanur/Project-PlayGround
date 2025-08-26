// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class APlaygroundPlayerWeapon;
/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayerCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Playground|Combat")
	APlaygroundPlayerWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;
};
