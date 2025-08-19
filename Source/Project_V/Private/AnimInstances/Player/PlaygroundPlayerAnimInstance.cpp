// Copyright 2025. Jinsol Co. All rights reserved


#include "AnimInstances/Player/PlaygroundPlayerAnimInstance.h"
#include "Characters/PlaygroundPlayerCharacter.h"

void UPlaygroundPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningPlayerCharacter = Cast<APlaygroundPlayerCharacter>(OwningCharacter);
	}
}

void UPlaygroundPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElpasedTime += DeltaSeconds;

		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
	}
}