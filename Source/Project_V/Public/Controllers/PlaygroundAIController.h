// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlaygroundAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class PROJECT_V_API APlaygroundAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	APlaygroundAIController(const FObjectInitializer& ObjectInitializer);

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
