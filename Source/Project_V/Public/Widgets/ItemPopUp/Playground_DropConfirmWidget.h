// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Playground_DropConfirmWidget.generated.h"

/**
 * 
 */
class UButton;
class UTextBlock;
class USizeBox;

DECLARE_DYNAMIC_DELEGATE(FDropConfirm);
DECLARE_DYNAMIC_DELEGATE(FDropCancel);

UCLASS()
class PROJECT_V_API UPlayground_DropConfirmWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	void SetInfo(const FText& ItemName, int32 Count);
	
	FVector2D PG_DropsGetBoxSize() const;

	FDropConfirm OnConfirm;
	FDropCancel OnCancel;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Confirm;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Cancel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UFUNCTION()
	void PG_ConfirmClicked();

	UFUNCTION()
	void PG_CancelClicked();

};
