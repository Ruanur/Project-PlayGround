// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Items/Drops/Manifest/Playground_ItemManifest.h"
#include "UPlayground_ItemRegistry.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UUPlayground_ItemRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const FPlayground_ItemManifest* FindManifestByID(FName ItemID) const;
	

private:
	UPROPERTY()
	TMap<FName, FPlayground_ItemManifest> ManifestMap;
};
