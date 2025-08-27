// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PlaygroundGameplayAbility.h"

void UPlaygroundAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UPlaygroundAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{

}

void UPlaygroundAbilitySystemComponent::GrantPlayerWeaponAbilities(
	const TArray<FPlaygroundPlayerAbilitySet>& InDefaultWeaponAbilities, 
	int32 ApplyLevel, 
	TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	for (const FPlaygroundPlayerAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		
		GiveAbility(AbilitySpec);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));

	}
}
