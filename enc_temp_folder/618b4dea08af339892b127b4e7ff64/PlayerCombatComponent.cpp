// Copyright 2025. Jinsol Co. All rights reserved


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Weapons/PlaygroundPlayerWeapon.h"

#include "PlaygroundDebugHelper.h"
APlaygroundPlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<APlaygroundPlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT("Hit ") + HitActor->GetActorNameOrLabel(), FColor::Green);
}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
    Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT("'s Weapon Pulled From ") + InteractedActor->GetActorNameOrLabel(), FColor::Green);
}
