// Copyright 2025. Jinsol Co. All rights reserved


#include "PlayergroundTypes/PlaygroundStructTypes.h"
#include "AbilitySystem/Abilities/PlaygroundGameplayAbility.h"

bool FPlaygroundPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}