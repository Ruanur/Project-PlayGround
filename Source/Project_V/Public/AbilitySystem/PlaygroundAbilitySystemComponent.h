// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PlayergroundTypes/PlaygroundStructTypes.h"
#include "PlaygroundAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "Playground|Ability", meta = (ApplyLevel = "1"))
	void GrantPlayerWeaponAbilities(const TArray<FPlaygroundPlayerAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
};
