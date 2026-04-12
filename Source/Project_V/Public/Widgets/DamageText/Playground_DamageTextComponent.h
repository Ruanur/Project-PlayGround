// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Playground_DamageTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayground_DamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);
};
