// Copyright 2025. Jinsol Co. All rights reserved


#include "AI/BTService_OrientToTargetActor.h"
#include "BehaviorTree/BlackboardComponent.h" //블랙보드 값 접근용
#include "AIController.h" // AIController 참조
#include "Kismet/KismetMathLibrary.h" 

UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	//노드 이름 (Behavior Tree 에디터에서 보여지는 이름)
	NodeName = TEXT("Native Orient Rotation To Target Actor");

	// 서비스 노드의 알림(flag) 초기화
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	// 회전 속도 보간값(높을수록 빠르게 회전)
	RotationInterpSpeed = 5.f;

	//Tick 주기 (0이면 매 프레임 Tick)
	Interval = 0.f;

	//랜덤 회전 편차 
	RandomDeviation = 0.f;

	// BlackBoard에서 가져올 객체 키 필터링
	// "InTargetActorKey"는 반드시 AActor 타입이여야 함
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}


// ========================================
// 블랙보드 초기화
// ========================================

void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	//부모 초기화 호출
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}


// ========================================
// 에디터에서 노드 설명 표시
// ========================================
FString UBTService_OrientToTargetActor::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Orient rotation to %s Key %s"),
		*KeyDescription,
		*GetStaticServiceDescription()
	);
}

// ========================================
// 매 Tick 호출: Pawn을 Target Actor 방향으로 회전
// ========================================
void UBTService_OrientToTargetActor::TickNode(
	UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory, 
	float DeltaSeconds
)
{
	// 부모 Tick 호출 (기본 서비스 Tick)
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	//Pawn(Enemy)와 TargetActor(User)가 유효할 때 회전
	if (OwningPawn && TargetActor)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

		OwningPawn->SetActorRotation(TargetRot);
	}
}