// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/Abilities/PlaygroundGameplayAbility.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PlaygroundFunctionLibrary.h"
#include "PlaygroundGameplayTags.h"

void UPlaygroundGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec); // 부모 로직도 실행하고 싶다면 호출
    
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
    // 부모 클래스 기본 동작 유지하고 싶으면
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

FActiveGameplayEffectHandle UPlaygroundGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    if (!TargetASC)
    {
        UE_LOG(LogTemp, Error, TEXT("TargetASC is NULL. TargetActor: %s"), *GetNameSafe(TargetActor));
    }

    if (!InSpecHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("SpecHandle is INVALID."));
    }

    check(TargetASC && InSpecHandle.IsValid());

    return GetPlaygroundAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *InSpecHandle.Data,
        TargetASC
    );
}

FActiveGameplayEffectHandle UPlaygroundGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EPlaygroundSuccessType& OutSuccessType)
{
    FActiveGameplayEffectHandle ActiveGameplayEffectHandle =  NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

    OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EPlaygroundSuccessType::Successful : EPlaygroundSuccessType::Failed;

    return ActiveGameplayEffectHandle;
}

void UPlaygroundGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
    if (InHitResults.IsEmpty())
    {
        return;
    }

    APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());

    for (const FHitResult& Hit : InHitResults)
    {
        if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
        {
            if (UPlaygroundFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
            {
                FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(HitPawn, InSpecHandle);

                if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
                {
                    FGameplayEventData Data;
                    Data.Instigator = OwningPawn;
                    Data.Target = HitPawn;

                    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
                        HitPawn,
                        PlaygroundGameplayTags::Shared_Event_HitReact,
                        Data
                    );
                }
            }
        }
    }
}


