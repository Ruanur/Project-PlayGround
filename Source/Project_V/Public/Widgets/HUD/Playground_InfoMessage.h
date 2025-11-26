// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Playground_InfoMessage.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class PROJECT_V_API UPlayground_InfoMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void MessageShow();

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void MessageHide();

	void PG_SetMessage(const FText& Message);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Messages")
	FText NoRoomMessage;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Message;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float MessageLifetime{ 3.f };

	FTimerHandle MessageTimer;
	bool bIsMessageActive{ false };
};
