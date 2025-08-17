// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;


// -----------------------------
// 입력 액션 설정을 나타내는 구조체
// (GameplayTag - InputAction 쌍을 정의)
// -----------------------------
USTRUCT(BlueprintType)
struct FPlaygroundInputActionConfig
{
	GENERATED_BODY()


public:
	// 입력을 구분하기 위한 GameplayTag
	// 예: InputTag.Move.Forward, InputTag.Attack.Light
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;
};
// -----------------------------
// 입력 설정을 Data Asset으로 관리하는 클래스
// -----------------------------
UCLASS()
class PROJECT_V_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 기본 입력 매핑 컨텍스트
	// (Input Mapping Context: 키보드/패드 등의 입력 바인딩 정의)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	// 프로젝트에서 사용할 기본 입력 액션 목록
	// - 태그와 액션 쌍으로 구성
	// - TitleProperty = "InputTag" : 에디터에서 InputTag가 리스트 제목으로 표시됨
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FPlaygroundInputActionConfig> NativeInputActions;

	// 전달받은 GameplayTag에 해당하는 InputAction을 반환
	// - 배열에서 InputTag를 검색
	// - 일치하는 항목이 없거나 유효하지 않으면 nullptr 반환
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;
};
