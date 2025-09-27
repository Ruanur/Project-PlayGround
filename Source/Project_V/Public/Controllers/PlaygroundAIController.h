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
	// FObjectInitializer
	APlaygroundAIController(const FObjectInitializer& ObjectInitializer);

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface

protected:
	virtual void BeginPlay() override;

	// UAIPerceptionComponent = AI 기능 중 감지역할
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	// UAISenseConfig_Sight = AI 기능 중 시야 감지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config");
	bool bEnableDetourCrowdAvoidance = true;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"));
	int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"));
	float CollisionQueryRange = 600.f;
};
