// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "PlaygroundStructTypes.generated.h"

class UPlaygroundPlayerLinkedAnimLayer;
class UPlaygroundPlayerGameplayAbility;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FPlaygroundPlayerAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlaygroundPlayerGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FPlaygroundPlayerSpecialAbilitySet : public FPlaygroundPlayerAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> SoftAbilityIconMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Player.Cooldown"))
	FGameplayTag AbilityCooldownTag; 
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FPlaygroundPlayerSpecialAbilitySet> SpecialWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};

USTRUCT(BlueprintType)
struct FPlayground_QuickSlotRef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid InstanceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID = NAME_None;

};