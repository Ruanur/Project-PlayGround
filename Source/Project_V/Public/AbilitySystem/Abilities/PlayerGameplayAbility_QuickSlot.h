// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlaygroundPlayerGameplayAbility.h"
#include "PlayerGameplayAbility_QuickSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayerGameplayAbility_QuickSlot : public UPlaygroundPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UPlayerGameplayAbility_QuickSlot();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
private:
	int32 ResolveSlotIndexFromEventTag(const struct FGameplayTag& EventTag) const;

};
