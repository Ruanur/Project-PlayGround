// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/Abilities/PlayerGameplayAbility_QuickSlot.h"

#include "PlaygroundGameplayTags.h"
#include "QuickSlot/Playground_QuickSlotComponent.h"
#include "AbilitySystemComponent.h"

UPlayerGameplayAbility_QuickSlot::UPlayerGameplayAbility_QuickSlot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(PlaygroundGameplayTags::Player_Ability_QuickSlot_1);
	AbilityTags.AddTag(PlaygroundGameplayTags::Player_Ability_QuickSlot_2);
	AbilityTags.AddTag(PlaygroundGameplayTags::Player_Ability_QuickSlot_3);
	AbilityTags.AddTag(PlaygroundGameplayTags::Player_Ability_QuickSlot_4);

	AbilityTriggers.Reset();

	auto AddTrigger = [this](const FGameplayTag& Tag)
		{
			FAbilityTriggerData Trigger;
			Trigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
			Trigger.TriggerTag = Tag;
			AbilityTriggers.Add(Trigger);
		};

	AddTrigger(PlaygroundGameplayTags::Player_Event_QuickSlot_1);
	AddTrigger(PlaygroundGameplayTags::Player_Event_QuickSlot_2);
	AddTrigger(PlaygroundGameplayTags::Player_Event_QuickSlot_3);
	AddTrigger(PlaygroundGameplayTags::Player_Event_QuickSlot_4);
}

void UPlayerGameplayAbility_QuickSlot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo || !ActorInfo, ActivationInfo, true, true)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (ASC->HasMatchingGameplayTag(PlaygroundGameplayTags::Shared_Status_Dead))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FGameplayTag EventTag = TriggerEventData ? TriggerEventData->EventTag : FGameplayTag();
	const int32 SlotIndex = ResolveSlotIndexFromEventTag(EventTag);
	if (SlotIndex == INDEX_NONE)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UPlayground_QuickSlotComponent* QS = nullptr;

	if (AActor* Avatar = ActorInfo->AvatarActor.Get())
	{
		QS = Avatar->FindComponentByClass<UPlayground_QuickSlotComponent>();
	}

	if (!QS && ActorInfo->PlayerController.IsValid())
	{
		QS = ActorInfo->PlayerController->FindComponentByClass<UPlayground_QuickSlotComponent>();
	}

	if (!QS)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	QS->UseSlot(SlotIndex);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

}

int32 UPlayerGameplayAbility_QuickSlot::ResolveSlotIndexFromEventTag(const FGameplayTag& EventTag) const
{
	if (EventTag.MatchesTagExact(PlaygroundGameplayTags::Player_Event_QuickSlot_1)) return 0;
	if (EventTag.MatchesTagExact(PlaygroundGameplayTags::Player_Event_QuickSlot_2)) return 1;
	if (EventTag.MatchesTagExact(PlaygroundGameplayTags::Player_Event_QuickSlot_3)) return 2;
	if (EventTag.MatchesTagExact(PlaygroundGameplayTags::Player_Event_QuickSlot_4)) return 3;

	return INDEX_NONE;
}


