// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "PlaygroundSaveGame.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	EPlaygroundGameDifficulty SavedCurrentGameDifficulty;
};
