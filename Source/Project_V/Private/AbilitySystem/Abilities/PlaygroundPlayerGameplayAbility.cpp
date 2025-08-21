// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/Abilities/PlaygroundPlayerGameplayAbility.h"
#include "Characters/PlaygroundPlayerCharacter.h"
#include "Controllers/PlayGroundPlayerController.h"

APlaygroundPlayerCharacter* UPlaygroundPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
    if (!CachedPlaygroundPlayerCharacter.IsValid())
    {
        CachedPlaygroundPlayerCharacter = Cast<APlaygroundPlayerCharacter>(CurrentActorInfo->AvatarActor);
    }

    return CachedPlaygroundPlayerCharacter.IsValid()? CachedPlaygroundPlayerCharacter.Get() : nullptr;
}

APlayGroundPlayerController* UPlaygroundPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
    if (CachedPlaygroundPlayerController.IsValid())
    {
        CachedPlaygroundPlayerController = Cast<APlayGroundPlayerController>(CurrentActorInfo->PlayerController);
    }
    return CachedPlaygroundPlayerController.IsValid() ? CachedPlaygroundPlayerController.Get() : nullptr;
}

UPlayerCombatComponent* UPlaygroundPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
    return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}
