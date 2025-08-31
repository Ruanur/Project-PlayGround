// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlaygroundGameplayAbility.h"
#include "PlaygroundEnemyGameplayAbility.generated.h"


class APlaygroundEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundEnemyGameplayAbility : public UPlaygroundGameplayAbility
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintPure, Category = "Playground|Ability")
	APlaygroundEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Playground|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<APlaygroundEnemyCharacter> CachedPlaygroundEnemyCharacter;
};
