// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"

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
