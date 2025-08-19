// Copyright 2025. Jinsol Co. All rights reserved


#include "Characters/PlaygroundBaseCharacter.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "AbilitySystem/PlaygroundAttributeSet.h"

// Sets default values
APlaygroundBaseCharacter::APlaygroundBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	PlaygroundAbilitySystemComponent = CreateDefaultSubobject<UPlaygroundAbilitySystemComponent>(TEXT("PlaygroundAbilitySystemComponent"));

	PlaygroundAttributeSet = CreateDefaultSubobject<UPlaygroundAttributeSet>(TEXT("PlaygroundAttributeSet"));
}

UAbilitySystemComponent* APlaygroundBaseCharacter::GetAbilitySystemComponent() const
{
	return GetPlaygroundAbilitySystemComponent();
}

void APlaygroundBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (PlaygroundAbilitySystemComponent)
	{
		PlaygroundAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}


