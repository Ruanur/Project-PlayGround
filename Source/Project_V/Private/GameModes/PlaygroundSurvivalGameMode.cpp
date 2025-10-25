// Copyright 2025. Jinsol Co. All rights reserved


#include "GameModes/PlaygroundSurvivalGameMode.h"

void APlaygroundSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void APlaygroundSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlaygroundSurvivalGameMode::SetCurrentSurvivalGameModeState(EPlaygroundSurvivalGameModeState InState)
{
	CurrentSurvivalGameModeState = InState;

	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}
