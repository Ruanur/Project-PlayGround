// Copyright 2025. Jinsol Co. All rights reserved


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpDataBase.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PlaygroundEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpDataBase::GiveToAbilitySystemComponent(UPlaygroundAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (EnemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UPlaygroundEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
		{
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
