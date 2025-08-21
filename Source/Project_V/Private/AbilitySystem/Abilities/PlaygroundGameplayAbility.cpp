// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/Abilities/PlaygroundGameplayAbility.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"

void UPlaygroundGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec); // �θ� ������ �����ϰ� �ʹٸ� ȣ��
    
    if (AbilityActivationPolicy == EPlaygroundAbilityActivationPolicy::OnGiven)
    {
        if (ActorInfo && !Spec.IsActive())
        {
            ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
        }
    }
}

void UPlaygroundGameplayAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    // �θ� Ŭ���� �⺻ ���� �����ϰ� ������
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (AbilityActivationPolicy == EPlaygroundAbilityActivationPolicy::OnGiven)
    {
        if (ActorInfo)
        {
            ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
        }
    }
   
}

UPawnCombatComponent* UPlaygroundGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
    return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UPlaygroundAbilitySystemComponent* UPlaygroundGameplayAbility::GetPlaygroundAbilitySystemComponentFromActorInfo() const
{
    return Cast<UPlaygroundAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}
