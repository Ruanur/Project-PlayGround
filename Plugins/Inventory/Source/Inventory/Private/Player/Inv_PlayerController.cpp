// 플레이어 입력 처리 + 월드 아이템 감지 + 인벤토리 UI 관리.
// Important Merge: 라인트레이스 기반 상호작용 로직이
// 기존 프로젝트의 Interaction / GA 시스템과 중복됨.
// → 현재 클래스에서 직접 상호작용 실행하는 부분을 "Interaction System"으로 위임하거나,
//   UInv_ItemComponent가 기존 인터랙션 인터페이스(IInteractable 등)를 구현하도록 병합하는 게 핵심.

#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/Inv_Highlightable.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/Inv_HUDWidget.h"

AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	//화면 중앙에서 일정 거리까지 Trace 거리
	TraceLength = 500.0;

	//ItemDetection 채널(프로젝트 세팅에 따로 구성 필요)
	ItemTraceChannel = ECC_GameTraceChannel1;
}

void AInv_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//프레임마다 아이템 감지
	//기존 Interaction System과 병합 시
	//TraceForItem은 타겟 감지 전달만 수행하고
	//실제 상호작용 실행은 기존 InteractionSystem이 수행하도록 해야함
	TraceForItem();
}

void AInv_PlayerController::ToggleInventory()
{
	if (!InventoryComponent.IsValid()) return;

	// 인벤토리 열기 닫기 토글
	InventoryComponent->ToggleInventoryMenu();

	// 인벤토리를 열면 HUD 숨김, 닫으면 다시 표시
	if (InventoryComponent->IsMenuOpen())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Enhanced Input 매핑 컨텍스트 등록
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(DefaultIMC, 0);
	}

	//플레이어가 가진 인벤토리 컴포넌트 찾기
	InventoryComponent = FindComponentByClass<UInv_InventoryComponent>();

	CreateHUDWidget();
}

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	//E Key -> Primary Interact
	// Merge Point : 
	// 이 라인에서 직접 TryAddItem()을 호출하기보다,
	// 기존 상호작용 시스템 (Ex. InteractionComponent->Interact()) 호출로 대체하는 것을 추천
	// 기존 시스템이 현재 바라보는 액터에 맞는 적절한 상호작용을(GA/기능) 실행 
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInv_PlayerController::PrimaryInteract);

	//인벤토리 열기/닫기
	EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AInv_PlayerController::ToggleInventory);
}

void AInv_PlayerController::PrimaryInteract()
{
	// 현재 트레이스 된 액터 없음
	if (!ThisActor.IsValid()) return;

	// 아이템 컴포넌트 찾기
	UInv_ItemComponent* ItemComp = ThisActor->FindComponentByClass<UInv_ItemComponent>();

	// Level 1
	// Current : 아이템 줍기 로직을 여기서 직접 실행
	// To Merge : 기존 상호작용 시스템에 위임
	// EX. InteractionSystem -> TryInteract(ThisActor)
	// 그리고 ItemComponent는 IInteractInterface 구현 후 Interact() 내부에서 TryAddItem 호출
	if (!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;

	//Current : 바로 인벤토리에 아이템 추가
	InventoryComponent->TryAddItem(ItemComp);
}

void AInv_PlayerController::CreateHUDWidget()
{
	if (!IsLocalController()) return;
	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void AInv_PlayerController::TraceForItem()
{
	// Viewport 크기 확인
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;

	//화면 중심 -> 월드 방향으로 변환
	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;

	// Line Trace
	const FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	// 이전 프레임의 액터 저장
	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	// 감지된 액터가 없으면 메세지 숨김
	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
	}

	// 같은 액터를 바라보고 있다면 처리하지 않음
	if (ThisActor == LastActor) return;

	// 새롭게 바라보는 액터에 대해
	if (ThisActor.IsValid())
	{
		//인터페이스가 있으면 하이라이트
		if (UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_Highlight(Highlightable);
		}
		
		// 아이템이라면 HUD 메시지 표시
		// To Merge : 아이템 뿐 아니라 일반 Interacter도 메시지를 띄워야 함
		UInv_ItemComponent* ItemComponent = ThisActor->FindComponentByClass<UInv_ItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
	}
	
	// 이전 액터의 하이라이트 해제
	if (LastActor.IsValid())
	{
		if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_UnHighlight(Highlightable);
		}
	}
}
