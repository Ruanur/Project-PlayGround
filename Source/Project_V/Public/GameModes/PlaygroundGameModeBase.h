// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "PlaygroundGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API APlaygroundGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APlaygroundGameModeBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EPlaygroundGameDifficulty CurrentGameDifficulty;

public:
	FORCEINLINE EPlaygroundGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
};
