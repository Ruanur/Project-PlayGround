// Copyright 2025. Jinsol Co. All rights reserved


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Weapons/PlaygroundPlayerWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PlaygroundGameplayTags.h"

#include "PlaygroundDebugHelper.h"
APlaygroundPlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<APlaygroundPlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }

    OverlappedActors.AddUnique(HitActor);

    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = HitActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningPawn(),
        PlaygroundGameplayTags::Shared_Event_MeleeHit,
        Data
    );
}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
    
}
