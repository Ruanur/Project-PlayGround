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
 * �÷��̾� ĳ���� (APlaygroundBaseCharacter ���)
 *
 * ���:
 * - ī�޶�(��/�ȷο� ī�޶�) ����
 * - �̵�/���� �Է� ó��
 * - Ability System �Է� ����
 * - PlayerCombatComponent ����
 */

UCLASS()
class PROJECT_V_API APlaygroundPlayerCharacter : public APlaygroundBaseCharacter
{
	GENERATED_BODY()

public:
	APlaygroundPlayerCharacter();
	
protected:
	//�÷��̾ Controller�� ������ �� ȣ��
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	//�Է� ���ε�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	
private:
#pragma region Components
	
	//3��Ī ī�޶� �����ϱ� ���� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"));
	USpringArmComponent* CameraBoom;

	//���� ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"));
	UCameraComponent* FollowCamera;

	// �÷��̾� ���� ���� ��� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"));
	UPlayerCombatComponent* PlayerCombatComponent;

#pragma endregion

#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"));
	UDataAsset_InputConfig* InputConfigDataAsset;

	//�̵� �Է� ó�� (WASD)
	void Input_Move(const FInputActionValue& InputActionValue);

	//���� �Է� ó�� (���콺)
	void Input_Look(const FInputActionValue& InputActionValue);

	//�����Ƽ �Է� (����)
	void Input_AbilityInputPressed(FGameplayTag InInputTag);

	//�����Ƽ �Է� (��)
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

#pragma endregion

public:
	FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }

};
