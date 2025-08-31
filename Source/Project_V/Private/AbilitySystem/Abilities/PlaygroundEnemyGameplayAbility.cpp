// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/Abilities/PlaygroundEnemyGameplayAbility.h"
#include "Characters/PlaygroundEnemyCharacter.h"

APlaygroundEnemyCharacter* UPlaygroundEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    if (!CachedPlaygroundEnemyCharacter.IsValid())
    {
        CachedPlaygroundEnemyCharacter = Cast<APlaygroundEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }

    return CachedPlaygroundEnemyCharacter.IsValid() ? CachedPlaygroundEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UPlaygroundEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
