// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/Abilities/PlaygroundEnemyGameplayAbility.h"
#include "Characters/PlaygroundEnemyCharacter.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "PlaygroundGameplayTags.h"

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

FGameplayEffectSpecHandle UPlaygroundEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
    check(EffectClass);

    FGameplayEffectContextHandle ContextHandle = GetPlaygroundAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetPlaygroundAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        ContextHandle
    );

    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        PlaygroundGameplayTags::Shared_SetByCaller_BaseDamage,
        InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
    );

    return EffectSpecHandle;
}
