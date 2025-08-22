// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlaygroundBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "PlaygroundPlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UPlayerCombatComponent;

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
	
protected:
	//플레이어가 Controller에 소유될 때 호출
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	//입력 바인딩
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	
private:
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

#pragma endregion

#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"));
	UDataAsset_InputConfig* InputConfigDataAsset;

	//이동 입력 처리 (WASD)
	void Input_Move(const FInputActionValue& InputActionValue);

	//시점 입력 처리 (마우스)
	void Input_Look(const FInputActionValue& InputActionValue);

	//어빌리티 입력 (누름)
	void Input_AbilityInputPressed(FGameplayTag InInputTag);

	//어빌리티 입력 (뗌)
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

#pragma endregion

public:
	FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }

};
