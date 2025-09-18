// Copyright 2025. Jinsol Co. All rights reserved


#include "Controllers/PlaygroundAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "PlaygroundDebugHelper.h"

//Path Following 동작 커스터마이징, Crowd 버전으로 교체함으로 여러 AI가 동시에 움직일 때 충돌없이 자연스레 회피하며 이동
// UPathFollowingComponent : 개별 이동 처리, Crowd: 집단 단위의 지능적인 이동 패턴 구현
APlaygroundAIController::APlaygroundAIController(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("CrowdFollowingComponent valid"), FColor::MakeRandomColor());
	}

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 5000.f;
	//플레이어 놓치지 않음
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	//시야각
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);
}

void APlaygroundAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

}
