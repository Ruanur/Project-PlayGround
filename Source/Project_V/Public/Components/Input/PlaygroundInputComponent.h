// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "PlaygroundInputComponent.generated.h"

/**
 * 입력 액션을 태그 기반으로 찾아서 바인딩하는 커스텀 Enhanced InputComponent.
 * InputAction 하드 코딩 방지
 * DataAsset_InputConfig에 등록된 GameplayTag -> InputAction 매핑을 찾아 입력 함수 연결
 * 일반 입력과 Ability 입력을 구분하여 바인딩할 수 있도록 하기 위함
 * GAS/GameplayTag 기반 입력 구조 최적화
 * 
 * Project Setting -> Input Component 해당 컴포넌트로 설정 필요
 */
UCLASS()
class PROJECT_V_API UPlaygroundInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * 일반 Native InputAction을 GameplayTag 기반으로 바인딩한다.
	 *
	 * 예:
	 * - InputTag.Move
	 * - InputTag.Look
	 * - InputTag.Jump
	 *
	 * InInputConfig:
	 * - 입력 설정 DataAsset
	 * - GameplayTag와 InputAction 매핑 정보를 가지고 있음.
	 *
	 * InInputTag:
	 * - 찾고자 하는 입력 태그
	 *
	 * TriggerEvent:
	 * - Started, Triggered, Completed 등 입력 이벤트 타입
	 *
	 * ContextObject:
	 * - 실제 입력 콜백 함수를 가지고 있는 객체
	 *
	 * Func:
	 * - 입력 발생 시 호출할 함수 포인터
	 */
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	/**
	 * Ability용 InputAction들을 한 번에 바인딩.
	 *
	 * DataAsset_InputConfig 안의 AbilityInputActions 배열을 순회하면서,
	 * 각 InputAction에 대해 Pressed / Released 이벤트를 연결한다.
	 *
	 * 이 함수는 보통 GAS와 함께 사용한다.
	 *
	 * 예:
	 * - InputTag.Ability.1
	 * - InputTag.Ability.2
	 * - InputTag.Ability.LMB
	 * - InputTag.Ability.RMB
	 *
	 * InputPressedFunc:
	 * - 입력이 시작되었을 때 호출할 함수
	 *
	 * InputReleasedFunc:
	 * - 입력이 끝났을 때 호출할 함수
	 *
	 * 주의:
	 * - 바인딩되는 함수는 FGameplayTag를 인자로 받을 수 있어야 한다.
	 * - BindAction의 마지막 인자로 InputTag를 넘기고 있기 때문이다.
	 */

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};

/**
 * 일반 입력 액션 바인딩 함수 구현
 *
 * 템플릿 함수는 cpp가 아니라 헤더에 구현해야 함.
 * 컴파일 시점에 실제 UserObject와 CallbackFunc 타입이 결정되기 때문
 */
template<class UserObject, typename CallbackFunc>
inline void UPlaygroundInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	// 입력 설정 DataAsset이 없으면 바인딩 진행 불가, 에러 처리
	checkf(InInputConfig, TEXT("Input config data aseet is null, can not proceed with binding"));

	// 전달받은 GameplayTag에 해당하는 Native InputAction을 DataAsset에서 검색
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		// 발견한 InputAction을 지정된 TriggerEvent와 콜백 함수에 바인딩
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}


/**
 * Ability 입력 액션 바인딩 함수 구현
 *
 * AbilityInputActions 배열에 등록된 모든 InputAction을 순회하면서,
 * Started 이벤트와 Completed 이벤트를 각각 Pressed / Released 함수에 연결한다.
 */
template<class UserObject, typename CallbackFunc>
inline void UPlaygroundInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	checkf(InInputConfig, TEXT("Input config data aseet is null, can not proceed with binding"));

	for (const FPlaygroundInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsValid())
		{
			continue;
		}

		// 입력이 시작되었을 때 호출
		//
		// 마지막 인자로 InputTag를 함께 넘긴다.
		// 따라서 InputPressedFunc는 FGameplayTag를 인자로 받을 수 있어야 한다.
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);

		// 입력이 끝났을 때 호출
		//
		// Ability의 Hold, Charge, Release 구조를 만들 때 유용하다.
		// 예: 버튼을 누르는 동안 조준, 버튼을 떼면 발사
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}
