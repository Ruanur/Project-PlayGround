// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;


// -----------------------------
// �Է� �׼� ������ ��Ÿ���� ����ü
// (GameplayTag - InputAction ���� ����)
// -----------------------------
USTRUCT(BlueprintType)
struct FPlaygroundInputActionConfig
{
	GENERATED_BODY()


public:
	// �Է��� �����ϱ� ���� GameplayTag
	// ��: InputTag.Move.Forward, InputTag.Attack.Light
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;
};
// -----------------------------
// �Է� ������ Data Asset���� �����ϴ� Ŭ����
// -----------------------------
UCLASS()
class PROJECT_V_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// �⺻ �Է� ���� ���ؽ�Ʈ
	// (Input Mapping Context: Ű����/�е� ���� �Է� ���ε� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	// ������Ʈ���� ����� �⺻ �Է� �׼� ���
	// - �±׿� �׼� ������ ����
	// - TitleProperty = "InputTag" : �����Ϳ��� InputTag�� ����Ʈ �������� ǥ�õ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FPlaygroundInputActionConfig> NativeInputActions;

	// ���޹��� GameplayTag�� �ش��ϴ� InputAction�� ��ȯ
	// - �迭���� InputTag�� �˻�
	// - ��ġ�ϴ� �׸��� ���ų� ��ȿ���� ������ nullptr ��ȯ
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;
};
