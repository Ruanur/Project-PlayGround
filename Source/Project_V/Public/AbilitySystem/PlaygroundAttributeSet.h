// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PlaygroundAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class IPawnUIInterface;

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	//속성 함수
	UPlaygroundAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;


	// Health
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UPlaygroundAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPlaygroundAttributeSet, MaxHealth)


	// Rage
	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UPlaygroundAttributeSet, CurrentRage)

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UPlaygroundAttributeSet, MaxRage)


	// Damage / Combat Stats
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UPlaygroundAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UPlaygroundAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UPlaygroundAttributeSet, DamageTaken)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(UPlaygroundAttributeSet, BaseDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BonusDamage;
	ATTRIBUTE_ACCESSORS(UPlaygroundAttributeSet, BonusDamage)

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
	void ShowFloatingText(const FGameplayEffectModCallbackData& Data, float Damage) const;
};
