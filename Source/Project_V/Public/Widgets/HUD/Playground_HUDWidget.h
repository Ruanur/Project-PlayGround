// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Playground_HUDWidget.generated.h"

/**
 * 
 */
class UPlayground_InfoMessage;

UCLASS() 
class PROJECT_V_API UPlayground_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayground_InfoMessage> InfoMessage;

	UFUNCTION()
	void PG_OnNoRoom();
};
