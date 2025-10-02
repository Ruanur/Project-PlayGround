// Copyright 2025. Jinsol Co. All rights reserved


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PlaygroundGameplayTags.h"
#include "PlaygroundFunctionLibrary.h"

#include "PlaygroundDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}
	
	OverlappedActors.AddUnique(HitActor);

	// TODO: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UPlaygroundFunctionLibrary::NativeDoesActorHaveTag(HitActor, PlaygroundGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UPlaygroundFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;


	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			PlaygroundGameplayTags::Player_Event_SuccessfulBlock,
			EventData
		);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			PlaygroundGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}
