// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Fragment/Playground_ItemFragment.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"

#include "PlaygroundDebugHelper.h"

// Get a Stats component from the PC or the PC->GetPawn()
// or get the Ability System Component and apply a Gameplay Effect
// or call an interface function for Healing()
void FPlayground_HealthPotionFragment::OnConsume(APlayerController* PC, UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	if (!PC) return;

	APawn* Pawn = PC->GetPawn();
	if (!Pawn) return;

	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC) return;

	const UGameplayEffect* EffectCDO =
		ConsumableGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	ASC->ApplyGameplayEffectToSelf(
		EffectCDO,
		1,
		ASC->MakeEffectContext()
	);
	
	Debug::Print(TEXT("Health Potion consumed %f"), FColor::Green);
}

void FPlayground_ManaPotionFragment::OnConsume(APlayerController* PC, UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	// Replenish mana however you wish

	Debug::Print(TEXT("Mana Potion consumed %f"), FColor::Blue, ManaAmount);
}
