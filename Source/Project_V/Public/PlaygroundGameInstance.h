// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "PlaygroundGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FPlaygroundGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "GameData.Level"))
	FGameplayTag LevelTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const
	{
		return LevelTag.IsValid() && !Level.IsNull();
	}


};

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlaygroundGameLevelSet> GameLevelSets;

public:
	UFUNCTION(BlueprintCallable, meta = (GameplayTagFliter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InTag);
	
};
