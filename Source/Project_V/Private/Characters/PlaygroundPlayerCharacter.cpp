// Copyright 2025. Jinsol Co. All rights reserved


#include "Characters/PlaygroundPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "PlaygroundDebugHelper.h"

 
APlaygroundPlayerCharacter::APlaygroundPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	//��Ʈ�ѷ��� ȸ���� ĳ���Ϳ� �������� ����
	//�����̴� �������� ĳ���Ͱ� ȸ���ϵ��� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	//ī�޶� �Ÿ��� ���� ����
	//SocketOffset: ī�޶� �� ��ġ ���� 
	//bUsePawnControlRotation: ��Ʈ�ѷ� ȸ���� ���� ī�޶� ȸ��
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void APlaygroundPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("Workinig"));
}
