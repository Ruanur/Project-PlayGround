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

	//컨트롤러의 회전을 캐릭터에 적용하지 않음
	//움직이는 방향으로 캐릭터가 회전하도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	//카메라 거리와 각도 조절
	//SocketOffset: 카메라 붐 위치 조정 
	//bUsePawnControlRotation: 컨트롤러 회전을 따라 카메라 회전
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
