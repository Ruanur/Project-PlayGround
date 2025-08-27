// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Weapons/PlaygroundPlayerWeapon.h"

void APlaygroundPlayerWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> APlaygroundPlayerWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
