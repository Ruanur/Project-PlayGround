// Copyright 2025. Jinsol Co. All rights reserved

//몬스터를 구현하고, AI가 이 몬스턱를 자동으로 제어하도록 설정
//데이터 에셋을 통해 캐릭터의 초기 Ability를 비동기적으로 로딩하는 역할 수행

#include "Characters/PlaygroundEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpDataBase.h"


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

}

UPawnCombatComponent* APlaygroundEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

void APlaygroundEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
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
