// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "PlayergroundTypes/PlaygroundStructTypes.h"
#include "DataAsset_PlayerStartUpData.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_V_API UDataAsset_PlayerStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UPlaygroundAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FPlaygroundPlayerAbilitySet> PlayerStartUpAbilitySets;
};
