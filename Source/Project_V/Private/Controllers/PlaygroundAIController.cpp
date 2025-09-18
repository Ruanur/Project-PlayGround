// Copyright 2025. Jinsol Co. All rights reserved


#include "Controllers/PlaygroundAIController.h"
#include "Navigation/CrowdFollowingComponent.h"

#include "PlaygroundDebugHelper.h"

APlaygroundAIController::APlaygroundAIController(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("CrowdFollowingComponent valid"), FColor::MakeRandomColor());
	}
}
