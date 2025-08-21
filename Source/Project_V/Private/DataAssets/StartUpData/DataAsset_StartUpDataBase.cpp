// Copyright 2025. Jinsol Co. All rights reserved


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PlaygroundGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UPlaygroundAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UPlaygroundGameplayAbility>>& InAbilitiesToGive, UPlaygroundAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UPlaygroundGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;


		InASCToGive->GiveAbility(AbilitySpec);
	}
}
