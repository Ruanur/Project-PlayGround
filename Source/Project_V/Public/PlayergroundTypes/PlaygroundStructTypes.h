// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "GameplayTagContainer.h"
#include "PlaygroundStructTypes.generated.h"

class UPlaygroundPlayerLinkedAnimLayer;
class UPlaygroundGameplayAbility;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FPlaygroundPlayerAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlaygroundGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FPlaygroundPlayerWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlaygroundPlayerLinkedAnimLayer> WeaponAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FPlaygroundPlayerAbilitySet> DefaultWeaponAbilities;
};
