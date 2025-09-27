// Copyright 2025. Jinsol Co. All rights reserved


#include "Components/Combat/EnemyCombatComponent.h"

#include "PlaygroundDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (HitActor)
	{
		Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT(" is hitting") + HitActor->GetActorNameOrLabel());
	}
}
