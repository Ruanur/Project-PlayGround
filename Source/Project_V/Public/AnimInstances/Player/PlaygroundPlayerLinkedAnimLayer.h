// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/PlaygroundBaseAnimInstance.h"
#include "PlaygroundPlayerLinkedAnimLayer.generated.h"

class UPlaygroundPlayerAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundPlayerLinkedAnimLayer : public UPlaygroundBaseAnimInstance
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintPure, meta = (NotBlueprintThreadSafe))
	UPlaygroundPlayerAnimInstance* GetPlayerAnimInstance() const;
};
