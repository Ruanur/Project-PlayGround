// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/Abilities/PlaygroundPlayerGameplayAbility.h"
#include "Characters/PlaygroundPlayerCharacter.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "PlaygroundGameplayTags.h"
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
    if (!CachedPlaygroundPlayerController.IsValid())
    {
        CachedPlaygroundPlayerController = Cast<APlayGroundPlayerController>(CurrentActorInfo->PlayerController);
    }

    return CachedPlaygroundPlayerController.IsValid()? CachedPlaygroundPlayerController.Get() : nullptr;
}

UPlayerCombatComponent* UPlaygroundPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
    return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

FGameplayEffectSpecHandle UPlaygroundPlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
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
        InWeaponBaseDamage
    );

    if (InCurrentAttackTypeTag.IsValid())
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
    }

    return EffectSpecHandle;
}

bool UPlaygroundPlayerGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime)
{
    check(InCooldownTag.IsValid());

    FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());

    TArray<TPair<float, float>> TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

    if (!TimeRemainingAndDuration.IsEmpty())
    {
        RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
        TotalCooldownTime = TimeRemainingAndDuration[0].Value;
    }

    return RemainingCooldownTime > 0.f;
}
