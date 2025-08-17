// Copyright 2025. Jinsol Co. All rights reserved


#include "DataAssets/Input/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	// NativeInputActions 배열을 순회하면서,
	// 요청된 GameplayTag와 일치하는 InputAction을 찾음.
	for (const FPlaygroundInputActionConfig& InputActionConfig : NativeInputActions)
	{
		// 조건 1: 입력 태그가 일치하는지 확인
		// 조건 2: 해당 InputAction 포인터가 유효한지 확인 (nullptr 아님)
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			// 조건을 만족하면 InputAction을 반환
			return InputActionConfig.InputAction;
		}
	}
	// 일치하는 InputAction이 없으면 nullptr 반환
	// (즉, 태그로 매핑된 입력 액션을 찾지 못했을 때)
	return nullptr;
}
