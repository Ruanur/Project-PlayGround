// Copyright 2025. Jinsol Co. All rights reserved


#include "Characters/PlaygroundPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/PlaygroundInputComponent.h"
#include "PlaygroundGameplayTags.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"

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

void APlaygroundPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (PlaygroundAbilitySystemComponent && PlaygroundAttributeSet)
	{
		const FString ASCText = FString::Printf(TEXT("Owner Actor: %s, AvatarActor: %s"), *PlaygroundAbilitySystemComponent->GetOwnerActor()->GetActorLabel(), *PlaygroundAbilitySystemComponent->GetAvatarActor()->GetActorLabel());

		Debug::Print(TEXT("Ability system component valid") + ASCText, FColor::Green);
		Debug::Print(TEXT("AttributeSet valid. ") + ASCText, FColor::Green);
	}
}

void APlaygroundPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UPlaygroundInputComponent* PlaygroundInputComponent = CastChecked<UPlaygroundInputComponent>(PlayerInputComponent);

	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
}

void APlaygroundPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlaygroundPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlaygroundPlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	//마우스 감도
	const float LookSensitivityYaw = 1.0f;
	const float LookSensitivityPitch = 1.0f;
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X * LookSensitivityYaw);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y * LookSensitivityPitch);
	}
}
