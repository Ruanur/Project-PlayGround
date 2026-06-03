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
#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameModes/PlaygroundGameModeBase.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "Widgets/HUD/Playground_HUDWidget.h"
#include "Controllers/PlayGroundPlayerController.h"
#include "Widgets/Spatial/Playground_InventoryGrid.h"
#include "QuickSlot/Playground_QuickSlotComponent.h"
#include "AbilitySystem/PlaygroundAttributeSet.h"

#include "PlaygroundDebugHelper.h"

 
APlaygroundPlayerCharacter::APlaygroundPlayerCharacter()
{
	// 캐릭터 충돌 캡슐 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	//컨트롤러의 회전을 캐릭터에 적용하지 않음
	// 움직이는 방향으로 캐릭터가 회전하도록 설정
	// 즉, 마우스를 움직여도 캐릭터의 몸통이 즉시 회전하지 않는다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	//카메라 거리와 각도 조절, 카메라 세팅
	//SocketOffset: 카메라 붐 위치 조정 
	//bUsePawnControlRotation: 컨트롤러 회전을 따라 카메라 회전
	//TODO : 줌 인, 줌 아웃 확장
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	//캡슐 컴포넌트에 부착, 충돌체 기준이라 위치 안정적, 흔들림 없음
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 105.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Combat Component 추가
	// 공격, 타겟팅, 무기 처리 등 플레이어 전투 관련 기능 담당
	PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));

	// 플레이어 전용 ui 컴포넌트 생성
	// HUD, 체력바 등 플레이어 UI 데이터 전달에 사용
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
}

UPawnCombatComponent* APlaygroundPlayerCharacter::GetPawnCombatComponent() const
{
	// IPawnCombatInterface를 통해 외부 시스템이 전투 컴포넌트에 접근할 수 있게 함.
	// 구체 클래스인 APlaygroundPlayerCharacter에 직접 의존하지 않게 만드는 구조
	return PlayerCombatComponent;
}

UPawnUIComponent* APlaygroundPlayerCharacter::GetPawnUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* APlaygroundPlayerCharacter::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}


// 플레이어가 Controller에 의해 소유될 때 호출
// 서버에서 ASC 초기화, Ability 부여, Attribute 초기화 등을 처리하기 좋은 시점
void APlaygroundPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//CharacterStartUpData 동기 로드 
	//플레이어 시작 시 어빌리티, 애트리뷰트 부여
	if (!CharacterStartUpData.IsNull())
	{
		// 동기 로드, StartUpData 즉시 불러옴
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			// Ability나 Attribute를 적용할 레벨
			// 난이도에 따라 다르게 적용하기 위함, 즉 보정치
			int32 AbilityApplyLevel = 1;

			// 현재 GameMode에서 난이도를 가져옴
			// GetAuthGameMode는 서버 권한에서만 유효함.
			if (APlaygroundGameModeBase* BaseGameMode = GetWorld()->GetAuthGameMode<APlaygroundGameModeBase>())
			{
				switch (BaseGameMode->GetCurrentGameDifficulty())
				{
				case EPlaygroundGameDifficulty::Easy:
					AbilityApplyLevel = 4;
					Debug::Print(TEXT("Current Difficulty: Easy"));
					break;

				case EPlaygroundGameDifficulty::Normal:
					AbilityApplyLevel = 3;
					Debug::Print(TEXT("Current Difficulty: Normal"));
					break;

				case EPlaygroundGameDifficulty::Hard:
					AbilityApplyLevel = 2;
					Debug::Print(TEXT("Current Difficulty: Hard"));
					break;

				case EPlaygroundGameDifficulty::VeryHard:
					AbilityApplyLevel = 1;
					Debug::Print(TEXT("Current Difficulty: Very Hard"));
					break;

				default:
					break;
				}
			}

			// StartUpData에 정의된 Ability, Attribute, Effect 등을
			// PlaygroundAbilitySytstemComponent에 부여함
			LoadedData->GiveToAbilitySystemComponent(PlaygroundAbilitySystemComponent, AbilityApplyLevel);
		}
	}
}

// 입력 바인딩 - Enhanced Input System
// Unreal이 Pawn InputComponent를 준비한 뒤 호출한다.
void APlaygroundPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 입력 설정 DataAsset이 반드지 지정되어야 함.
	// 이 DataAsset 안에 MappingContext와 InputTag -> InputAction 매핑 정보 들어있음
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	// 현재 캐릭터를 조종하는 PlayerController의 LocalPlayer를 가져옴
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	// Enhanced Input은 LocalPlayer SubSystem에 MappingContext를 등록해야 작동함
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	// DataAsset에 지정된 기본 MappingContext를 이 플레이어에게 등록한다.
	//
	// Ex.
	// IMC_Player 안에
	// WASD → IA_Move
	// Mouse → IA_Look
	// E → IA_Interact
	// 1 → IA_QuickSlot1
	// 같은 매핑이 포함되어 있음
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	// Unreal이 넘겨준 기본 UInputComponent를
	// 프로젝트 전용 UPlaygroundInputComponent로 캐스팅한다.
	//
	// 이 캐스팅이 성공해야 아래의
	// BindNativeInputAction()
	// BindAbilityInputAction()
	// 을 사용할 수 있다.
	//
	// 주의:
	// 실제 InputComponent 클래스가 UPlaygroundInputComponent가 아니라면
	// CastChecked에서 런타임 에러가 발생한다.
	UPlaygroundInputComponent* PlaygroundInputComponent = CastChecked<UPlaygroundInputComponent>(PlayerInputComponent);


	// =========================
	// Native Input 바인딩
	// =========================
	//
	// Native Input은 캐릭터가 직접 처리하는 입력이다.
	// 이동, 시점, 상호작용, 퀵슬롯처럼 캐릭터 함수로 바로 연결된다.
	//
	// 내부 흐름:
	// InputTag_Move
	// → InputConfigDataAsset에서 해당 태그의 InputAction 검색
	// → 찾은 InputAction의 Triggered 이벤트에 Input_Move 함수 바인딩
	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_PickUp_Stones, ETriggerEvent::Started, this, &ThisClass::Input_PickUpStonesStarted);
	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_Interact_Object, ETriggerEvent::Started, this, &ThisClass::Input_InteractionObjectStarted);
	//PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_Inventory, ETriggerEvent::Started, this, &ThisClass::Input_Inventory);

	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_QuickSlot_1, ETriggerEvent::Started, this, &ThisClass::Input_UseQuickSlot1);
	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_QuickSlot_2, ETriggerEvent::Started, this, &ThisClass::Input_UseQuickSlot2);
	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_QuickSlot_3, ETriggerEvent::Started, this, &ThisClass::Input_UseQuickSlot3);
	PlaygroundInputComponent->BindNativeInputAction(InputConfigDataAsset, PlaygroundGameplayTags::InputTag_QuickSlot_4, ETriggerEvent::Started, this, &ThisClass::Input_UseQuickSlot4);

	PlaygroundInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(ToggleInventroyAction, ETriggerEvent::Started, this, &ThisClass::InventoryToggle);

	//EnhancedInputComponent->BindAction(UseQuickSlot1, ETriggerEvent::Started, this, &ThisClass::Input_UseQuickSlot1);
	//EnhancedInputComponent->BindAction(UseQuickSlot2, ETriggerEvent::Started, this, &ThisClass::Input_UseQuickSlot2);
	//EnhancedInputComponent->BindAction(UseQuickSlot3, ETriggerEvent::Started, this, &ThisClass::Input_UseQuickSlot3);
	//EnhancedInputComponent->BindAction(UseQuickSlot4, ETriggerEvent::Started, this, &ThisClass::Input_UseQuickSlot4);
}

void APlaygroundPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InventoryComponent = FindComponentByClass<UPlayground_InventoryComponent>();
	QuickSlotComponent = FindComponentByClass<UPlayground_QuickSlotComponent>();

	InventoryComponent->OnInventoryLoaded.Broadcast();
	PG_CreateHUDWidget();
}

void APlaygroundPlayerCharacter::PG_CreateHUDWidget()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	HUDWidget = CreateWidget<UPlayground_HUDWidget>(PC, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}

}

void APlaygroundPlayerCharacter::PG_PrimaryInteract()
{
	if (!ThisActor.IsValid()) return;

	UPlayground_ItemComponent* ItemComp = ThisActor->FindComponentByClass<UPlayground_ItemComponent>();
	if (!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;

	InventoryComponent->TryAddItem(ItemComp);
}

void APlaygroundPlayerCharacter::Input_UseQuickSlot1(const FInputActionValue& Value) 
{ 
	FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		PlaygroundGameplayTags::Player_Event_QuickSlot_1, Data
	);
	//UseQuickSlot(0);
}

void APlaygroundPlayerCharacter::Input_UseQuickSlot2(const FInputActionValue& Value) 
{
	FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		PlaygroundGameplayTags::Player_Event_QuickSlot_2, Data
	);
	//UseQuickSlot(1); 
}

void APlaygroundPlayerCharacter::Input_UseQuickSlot3(const FInputActionValue& Value) 
{
	FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		PlaygroundGameplayTags::Player_Event_QuickSlot_3, Data
	);
	//UseQuickSlot(2); 
}
void APlaygroundPlayerCharacter::Input_UseQuickSlot4(const FInputActionValue& Value) 
{
	FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		PlaygroundGameplayTags::Player_Event_QuickSlot_4, Data
	);
	//UseQuickSlot(3); 
}

void APlaygroundPlayerCharacter::RestoreFullHealthAfterEquipmentLoad()
{
	if (HasAuthority())
	{
		RestoreFullHealthAfterEquipmentLoad_Internal();
	}
	else
	{
		Server_RestoreFullHealthAfterEquipmentLoad();
	}
}

void APlaygroundPlayerCharacter::Server_RestoreFullHealthAfterEquipmentLoad_Implementation()
{
	RestoreFullHealthAfterEquipmentLoad_Internal();
}

void APlaygroundPlayerCharacter::RestoreFullHealthAfterEquipmentLoad_Internal()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);
	if (!ASC) return;

	const UPlaygroundAttributeSet* AttributeSet = ASC->GetSet<UPlaygroundAttributeSet>();
	if (!AttributeSet) return;

	const float MaxHealthValue = AttributeSet->GetMaxHealth();

	//Debug::Print(FString::Printf(TEXT("Before Restore | Current=%.2f Max=%.2f"), AttributeSet->GetCurrentHealth(), AttributeSet->GetMaxHealth()));
	ASC->SetNumericAttributeBase(UPlaygroundAttributeSet::GetCurrentHealthAttribute(), MaxHealthValue);
	//Debug::Print(FString::Printf(TEXT("After Restore | TargetCurrent=%.2f"), MaxHealthValue));

	AttributeSet->RequestBroadcastHealthUI();

	Debug::Print(TEXT("Restore Full Health After Equipment Load"));
}

void APlaygroundPlayerCharacter::InventoryToggle()
{
	if (!InventoryComponent.IsValid()) return;
	InventoryComponent->ToggleInventoryMenu();
}

void APlaygroundPlayerCharacter::UseQuickSlot(int32 SlotIndex)
{
	if (!QuickSlotComponent.IsValid())
	{
		QuickSlotComponent = FindComponentByClass<UPlayground_QuickSlotComponent>();
	}

	UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Use Slot=%d"), SlotIndex);
	QuickSlotComponent->UseSlot(SlotIndex);
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

//마우스를 통한 회전
//감도 하드코딩 1.0 
// TODO: 환경 설정 내에서 설정 가능하게 확장
void APlaygroundPlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	//마우스 감도

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X * LookSensitivityYaw);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y * LookSensitivityPitch);
	}
}

void APlaygroundPlayerCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}


void APlaygroundPlayerCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X > 0.f ? PlaygroundGameplayTags::Player_Event_SwitchTarget_Right : PlaygroundGameplayTags::Player_Event_SwitchTarget_Left,
		Data
	);

	
}

void APlaygroundPlayerCharacter::Input_PickUpStonesStarted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		PlaygroundGameplayTags::Player_Event_ConsumeStones,
		Data
	);
}

void APlaygroundPlayerCharacter::Input_InteractionObjectStarted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		PlaygroundGameplayTags::Player_Event_InteractObject,
		Data
	);
}

//void APlaygroundPlayerCharacter::Input_Inventory(const FInputActionValue& InputActionValue)
//{
//	Debug::Print(TEXT("Arrived Inventory"));
//	InventoryToggle();
//}


//입력 이벤트 전달_ASC
void APlaygroundPlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	PlaygroundAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void APlaygroundPlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	PlaygroundAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}


