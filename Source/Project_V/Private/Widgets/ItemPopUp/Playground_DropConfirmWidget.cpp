// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/ItemPopUp/Playground_DropConfirmWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UPlayground_DropConfirmWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Confirm->OnClicked.AddDynamic(this, &ThisClass::PG_ConfirmClicked);
	Button_Cancel->OnClicked.AddDynamic(this, &ThisClass::PG_CancelClicked);
}

void UPlayground_DropConfirmWidget::SetInfo(const FText& ItemName, int32 Count)
{
	if (Text_ItemName) Text_ItemName->SetText(ItemName);
	if (Text_Count) Text_Count->SetText(FText::AsNumber(Count));
}

FVector2D UPlayground_DropConfirmWidget::PG_DropsGetBoxSize() const
{
	return FVector2D(SizeBox_Root->GetWidthOverride(), SizeBox_Root->GetHeightOverride());
}

void UPlayground_DropConfirmWidget::PG_ConfirmClicked()
{
	OnConfirm.ExecuteIfBound();
	RemoveFromParent();
}

void UPlayground_DropConfirmWidget::PG_CancelClicked()
{
	OnCancel.ExecuteIfBound();
	RemoveFromParent();
}
