// Copyright 2025. Jinsol Co. All rights reserved

//몬스터를 구현하고, AI가 이 몬스턱를 자동으로 제어하도록 설정
//데이터 에셋을 통해 캐릭터의 초기 Ability를 비동기적으로 로딩하는 역할 수행

#include "Characters/PlaygroundEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpDataBase.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/PlaygroundWidgeBase.h"
#include "Components/BoxComponent.h"

#include "PlaygroundDebugHelper.h"

APlaygroundEnemyCharacter::APlaygroundEnemyCharacter()
{
	//AI 컨트롤러가 이 몬스터를 자동으로 소유하도록 설정
	//월드에 배치하거나 스폰될 때 AI가 제어
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 컨트롤러의 Pitch, Roll, Yaw 회전을 사용하지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//이동 방향 바라보기
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	//전투 관련 로직을 처리할 UEnemyCombatComponent를 생성 후 몬스터에 추가
	//전투 코드를 분리하여 관리
	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");

	//Enemy UI 관리
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");

	//UI 위젯을 캐릭터 메시에 붙임
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
}

UPawnCombatComponent* APlaygroundEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* APlaygroundEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* APlaygroundEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void APlaygroundEnemyCharacter::BeginPlay()
{
	//게임 시작 시 HP바 위젯 초기화 후 해당 위젯에 자기 자신을 참조로 전달
	Super::BeginPlay();
	
	if (UPlaygroundWidgeBase* HealthWidget = Cast<UPlaygroundWidgeBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreatedWidget(this);

	}
}

void APlaygroundEnemyCharacter::PossessedBy(AController* NewController)
{
	//AIController가 이 Pawn을 소유할 때 호출됨, 이 때 초기 데이터 로딩(InitEnemyStartUpData) 트리거
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

#if WITH_EDITOR
void APlaygroundEnemyCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{ 
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandCollisionBoxAttachBoneName);
	}
}
#endif

void APlaygroundEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APlaygroundEnemyCharacter::InitEnemyStartUpData()
{
	//CharacterStartUpData가 유효한지 확인(데이터 에셋 할당 여부)
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	//UAssetManager 데이터 비동기 로드
	//로드가 끝나면 엔진이 미리 등록해 둔 콜백 함수 호출 
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		//로드가 완료되었을 때 실행될 콜백 함수(람다) 정의
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				//로드된 데이터 에셋 가져오기
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					//GiveToAbilitySystemComponent에 로드된 데이터 전달
					//이를 통해 초기 데이터 전달
					LoadedData->GiveToAbilitySystemComponent(PlaygroundAbilitySystemComponent);
				}
			}
		)
	);
}
