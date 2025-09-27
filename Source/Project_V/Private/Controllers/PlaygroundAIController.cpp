// Copyright 2025. Jinsol Co. All rights reserved


#include "Controllers/PlaygroundAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "PlaygroundDebugHelper.h"

//Path Following 동작 커스터마이징, Crowd 버전으로 교체함으로 여러 AI가 동시에 움직일 때 충돌없이 자연스레 회피하며 이동
// 이 함수는 본래 AIController 부모로부터 파생된 PathFollowingComponent를 가짐 -> Super를 통한 Crowd 버전으로 교체
// UPathFollowingComponent : 개별 이동 처리, Crowd: 집단 단위의 지능적인 이동 패턴 구현
APlaygroundAIController::APlaygroundAIController(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	//시야 정보를 통해 트리거
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 5000.f; //시야거리
	AISenseConfig_Sight->LoseSightRadius = 0.f; //플레이어 놓치지 않음
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; //시야각

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());

	//콜백 연결: 감지하거나 감지를 잃었을 때 호출 -> OnEnemyPerceptionUpdated
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}

// 다른 액터의 Team ID 확인, 다른 Team ID : Hostile, 같은 Team ID : Friendly
// Team 기반 적/아군 판별 시스템
ETeamAttitude::Type APlaygroundAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	//OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId() =>
	//OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId() 
	//벽을 보는 에러가 있음, 플레이어 팀 ID와 적 팀 ID를 대수 비교 하여 플레이어만 해당하게 변경
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	
	return ETeamAttitude::Friendly;
}

// Crowd Following 세부 설정
// - 회피 기능 : On/Off
// - 회피 품질(Low ~ High)
// - 회피 그룹 및 충돌 그룹 (군집 별 회피 구분 가능)
// - Collision Query Range (얼마나 멀리까지 충돌 검사할지)
// 
// AI 집단 이동의 충돌 회피와 품질 조정 담당
void APlaygroundAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);
		
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low); break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good); break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High); break;
		default:
			break;
		}

		//회피 그룹 1, 추후 보스 몬스터 그룹 int 값 설정 후 연출 구현
		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}

}

// 감지 성공 시:
// BlackBoard에 TargetActor를 저장 -> Behavior Tree가 접근
// Ai Perception -> Blackboard -> Behavior Tree PipeLine Connecting
// 감지 됐을 때 추가적인 효과를 삽입할 수도 있음
void APlaygroundAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}

	//if (Stimulus.WasSuccessfullySensed() && Actor)
	//{
	//	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	//	{
	//		BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
	//	}
	//}
}
