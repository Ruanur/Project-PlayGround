// Copyright 2025. Jinsol Co. All rights reserved


#include "AnimInstances/PlaygroundCharacterAnimInstance.h"
#include "Characters/PlaygroundBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlaygroundCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<APlaygroundBaseCharacter>(TryGetPawnOwner());

	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UPlaygroundCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}

