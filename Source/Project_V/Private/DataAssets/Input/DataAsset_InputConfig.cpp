// Copyright 2025. Jinsol Co. All rights reserved


#include "DataAssets/Input/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	// NativeInputActions �迭�� ��ȸ�ϸ鼭,
	// ��û�� GameplayTag�� ��ġ�ϴ� InputAction�� ã��.
	for (const FPlaygroundInputActionConfig& InputActionConfig : NativeInputActions)
	{
		// ���� 1: �Է� �±װ� ��ġ�ϴ��� Ȯ��
		// ���� 2: �ش� InputAction �����Ͱ� ��ȿ���� Ȯ�� (nullptr �ƴ�)
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			// ������ �����ϸ� InputAction�� ��ȯ
			return InputActionConfig.InputAction;
		}
	}
	// ��ġ�ϴ� InputAction�� ������ nullptr ��ȯ
	// (��, �±׷� ���ε� �Է� �׼��� ã�� ������ ��)
	return nullptr;
}
