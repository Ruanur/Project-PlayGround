// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Fragment/Playground_ItemFragment.h"

#include "PlaygroundDebugHelper.h"

void FPlayground_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	// Get a Stats component from the PC or the PC->GetPawn()
	// or get the Ability System Component and apply a Gameplay Effect
	// or call an interface function for Healing()
	
	Debug::Print(TEXT("Health Potion consumed %f"), FColor::Green, HealAmount);
}

void FPlayground_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	// Replenish mana however you wish

	Debug::Print(TEXT("Mana Potion consumed %f"), FColor::Blue, ManaAmount);
}
