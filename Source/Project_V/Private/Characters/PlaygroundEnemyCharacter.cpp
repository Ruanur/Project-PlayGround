// Copyright 2025. Jinsol Co. All rights reserved

//���͸� �����ϰ�, AI�� �� ���θ� �ڵ����� �����ϵ��� ����
//������ ������ ���� ĳ������ �ʱ� Ability�� �񵿱������� �ε��ϴ� ���� ����

#include "Characters/PlaygroundEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpDataBase.h"


#include "PlaygroundDebugHelper.h"
APlaygroundEnemyCharacter::APlaygroundEnemyCharacter()
{
	//AI ��Ʈ�ѷ��� �� ���͸� �ڵ����� �����ϵ��� ����
	//���忡 ��ġ�ϰų� ������ �� AI�� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// ��Ʈ�ѷ��� Pitch, Roll, Yaw ȸ���� ������� �ʵ��� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//�̵� ���� �ٶ󺸱�
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	//���� ���� ������ ó���� UEnemyCombatComponent�� ���� �� ���Ϳ� �߰�
	//���� �ڵ带 �и��Ͽ� ����
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
	//CharacterStartUpData�� ��ȿ���� Ȯ��(������ ���� �Ҵ� ����)
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	//UAssetManager ������ �񵿱� �ε�
	//�ε尡 ������ ������ �̸� ����� �� �ݹ� �Լ� ȣ�� 
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		//�ε尡 �Ϸ�Ǿ��� �� ����� �ݹ� �Լ�(����) ����
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				//�ε�� ������ ���� ��������
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					//GiveToAbilitySystemComponent�� �ε�� ������ ����
					//�̸� ���� �ʱ� ������ ����
					LoadedData->GiveToAbilitySystemComponent(PlaygroundAbilitySystemComponent);
				}
			}
		)
	);
}
