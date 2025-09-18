// Copyright 2025. Jinsol Co. All rights reserved


#include "Controllers/PlayGroundPlayerController.h"

APlayGroundPlayerController::APlayGroundPlayerController()
{
	PlayerTeamID = FGenericTeamId(0);
}

FGenericTeamId APlayGroundPlayerController::GetGenericTeamId() const
{
	return PlayerTeamID;
}