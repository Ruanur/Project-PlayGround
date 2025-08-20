// Copyright 2025. Jinsol Co. All rights reserved


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PlaygroundGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UPlaygroundAbilitySystemComponent* InPlayergroundASCToGive, int32 ApplyLevel)
{
	check(InPlayergroundASCToGive);

	GrantAbilities(ActivateOnGivenAbilities, InPlayergroundASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InPlayergroundASCToGive, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UPlaygroundGameplayAbility>>& InAbilitiesToGive, UPlaygroundAbilitySystemComponent* InPlayergroundASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UPlaygroundGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InPlayergroundASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;


		InPlayergroundASCToGive->GiveAbility(AbilitySpec);
	}
}
