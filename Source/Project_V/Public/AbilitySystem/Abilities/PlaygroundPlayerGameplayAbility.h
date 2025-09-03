// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlaygroundGameplayAbility.h"
#include "PlaygroundPlayerGameplayAbility.generated.h"

class APlaygroundPlayerCharacter;
class APlayGroundPlayerController;
/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundPlayerGameplayAbility : public UPlaygroundGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Playground|Ability")
	APlaygroundPlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Playground|Ability")
	APlayGroundPlayerController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Playground|Ability")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Playground|Ability")
	FGameplayEffectSpecHandle MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);


private:
	TWeakObjectPtr<APlaygroundPlayerCharacter> CachedPlaygroundPlayerCharacter;
	TWeakObjectPtr<APlayGroundPlayerController> CachedPlaygroundPlayerController;
};
