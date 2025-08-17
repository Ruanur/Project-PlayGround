// Copyright 2025. Jinsol Co. All rights reserved


#include "Characters/PlaygroundBaseCharacter.h"

// Sets default values
APlaygroundBaseCharacter::APlaygroundBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

}


