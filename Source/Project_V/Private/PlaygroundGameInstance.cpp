// Copyright 2025. Jinsol Co. All rights reserved


#include "PlaygroundGameInstance.h"

TSoftObjectPtr<UWorld> UPlaygroundGameInstance::GetGameLevelByTag(FGameplayTag InTag) const
{
	for (const FPlaygroundGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid()) continue;

		if (GameLevelSet.LevelTag == InTag)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}
