// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlaygroundBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "PlaygroundPlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
class UPlayerCombatComponent;
class UPlayerUIComponent;
class UPlayground_InventoryComponent;
class UInputAction;
class UPlayground_HUDWidget;
struct FInputActionValue;


/**
 * 플레이어 캐릭터 (APlaygroundBaseCharacter 상속)
 *
 * 기능:
 * - 카메라(붐/팔로우 카메라) 세팅
 * - 이동/시점 입력 처리
 * - Ability System 입력 전달
 * - PlayerCombatComponent 보유
 */

UCLASS()
class PROJECT_V_API APlaygroundPlayerCharacter : public APlaygroundBaseCharacter
{
	GENERATED_BODY()

public:
	APlaygroundPlayerCharacter();
	
	void CharacterRequestSaveInventory();

	//~ Begin IPawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface
	
	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
	//~ End IPawnUIInterface Interface.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float LookSensitivityYaw = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float LookSensitivityPitch = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InventoryToggle();
protected:
	//플레이어가 Controller에 소유될 때 호출
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	//입력 바인딩
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;


private:
//pragma region <> : 확장/축소 코드블록
	void PG_CreateHUDWidget();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UPlayground_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UPlayground_HUDWidget> HUDWidget;
#pragma region Components
	
	//3인칭 카메라를 제어하기 위한 스프링 암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"));
	USpringArmComponent* CameraBoom;

	//실제 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"));
	UCameraComponent* FollowCamera;

	// 플레이어 전투 로직 담당 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"));
	UPlayerCombatComponent* PlayerCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"));
	UPlayerUIComponent* PlayerUIComponent;

	TWeakObjectPtr<UPlayground_InventoryComponent> InventoryComponent;
#pragma endregion

#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"));
	UDataAsset_InputConfig* InputConfigDataAsset;

	UPROPERTY()
	FVector2D SwitchDirection = FVector2D::ZeroVector;

	//이동 입력 처리 (WASD)
	void Input_Move(const FInputActionValue& InputActionValue);

	//시점 입력 처리 (마우스)
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);

	void Input_PickUpStonesStarted(const FInputActionValue& InputActionValue);
	void Input_InteractionObjectStarted(const FInputActionValue& InputActionValue);
	void Input_Inventory(const FInputActionValue& InputActionValue);

	//어빌리티 입력 (누름)
	void Input_AbilityInputPressed(FGameplayTag InInputTag);

	//어빌리티 입력 (뗌)
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> ToggleInventroyAction;

#pragma endregion

#pragma region Inventory

	void PG_PrimaryInteract();
	TWeakObjectPtr<AActor> ThisActor;

#pragma endregion
public:
	FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }

};
