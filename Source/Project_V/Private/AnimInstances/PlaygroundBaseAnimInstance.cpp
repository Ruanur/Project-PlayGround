// Copyright 2025. Jinsol Co. All rights reserved


#include "AnimInstances/PlaygroundBaseAnimInstance.h"
#include "PlaygroundFunctionLibrary.h"

bool UPlaygroundBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UPlaygroundFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}

	return false;
}
