// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UPlaygroundGameplayAbility;
class UPlaygroundAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class PROJECT_V_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UPlaygroundAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UPlaygroundGameplayAbility>> ActivateOnGivenAbilities;


	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UPlaygroundGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffect;

	void GrantAbilities(const TArray<TSubclassOf<UPlaygroundGameplayAbility>>& InAbilitiesToGive, UPlaygroundAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

	
};
