// Copyright 2025. Jinsol Co. All rights reserved


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Weapons/PlaygroundPlayerWeapon.h"

APlaygroundPlayerWeapon* UPlayerCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<APlaygroundPlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}
