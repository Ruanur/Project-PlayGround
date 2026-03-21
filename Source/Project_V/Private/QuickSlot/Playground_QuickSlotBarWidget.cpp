// Copyright 2025. Jinsol Co. All rights reserved


#include "QuickSlot/Playground_QuickSlotBarWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "QuickSlot/Playground_QuickSlotComponent.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Items/Fragment/Playground_FragmentTags.h"
#include "Types/Playground_GridTypes.h"
#include "GameFramework/PlayerController.h"
#include "Items/Fragment/Playground_ItemFragment.h"

void UPlayground_QuickSlotBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CacheComponents();

	if (QS.IsValid())
	{
		QS->OnQuickSlotsChanged.AddDynamic(this, &ThisClass::HandleQuickSlotsChanged);
	}

	if (Inv.IsValid())
	{
		Inv->OnItemAdded.AddDynamic(this, &ThisClass::HandleItemAdded);
		Inv->OnStackChange.AddDynamic(this, &ThisClass::HandleStackChanged);
		//Inv->OnInventoryDataChanged.AddDynamic(this, &ThisClass::PG_RefreshUI);
	}

	PG_RefreshUI();
}

void UPlayground_QuickSlotBarWidget::NativeDestruct()
{
	if (QS.IsValid())
	{
		QS->OnQuickSlotsChanged.RemoveDynamic(this, &ThisClass::HandleQuickSlotsChanged);
	}

	if (Inv.IsValid())
	{
		Inv->OnItemAdded.RemoveDynamic(this, &ThisClass::HandleItemAdded);
		Inv->OnStackChange.RemoveDynamic(this, &ThisClass::HandleStackChanged);
	}

	Super::NativeDestruct();
}

void UPlayground_QuickSlotBarWidget::CacheComponents()
{
	AActor* OwnerActor = nullptr;

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		OwnerActor = Pawn;
	}
	else if (APlayerController* PC = GetOwningPlayer())
	{
		OwnerActor = PC;
	}

	if (!OwnerActor) return;

	QS = OwnerActor->FindComponentByClass<UPlayground_QuickSlotComponent>();
	Inv = OwnerActor->FindComponentByClass<UPlayground_InventoryComponent>();
}

void UPlayground_QuickSlotBarWidget::HandleQuickSlotsChanged()
{
	PG_RefreshUI();
}

void UPlayground_QuickSlotBarWidget::HandleItemAdded(UPlayground_InventoryItem* Item)
{
	PG_RefreshUI();
}

void UPlayground_QuickSlotBarWidget::HandleStackChanged(const FPlayground_SlotAvailabilityResult& Result)
{
	PG_RefreshUI();
}

void UPlayground_QuickSlotBarWidget::PG_RefreshUI()
{
	if (!QS.IsValid() || !Inv.IsValid())
	{
		CacheComponents();
		if (!QS.IsValid() || !Inv.IsValid())
		{
			return;
		}
	}

	const auto& Slots = QS->GetSlots();

	for (int32 i = 0; i < 4; ++i)
	{
		if (!Slots.IsValidIndex(i) || Slots[i].ItemID.IsNone())
		{
			SetSlotEmpty(i);
			continue;
		}

		const FName ItemID = Slots[i].ItemID;

		const int32 TotalCount = Inv->PG_GetTotalCountByItemID(ItemID);

		if (TotalCount <= 0)
		{
			SetSlotEmpty(i);
			continue;
		}

		UPlayground_InventoryItem* AnyItem = Inv->PG_FindFirstItemByItemID(ItemID);
		if (!IsValid(AnyItem))
		{
			SetSlotEmpty(i);
			continue;
		}

		SetSlotFromItem(i, AnyItem, TotalCount);
	}

}

void UPlayground_QuickSlotBarWidget::SetSlotEmpty(int32 SlotIndex)
{
	auto Set = [&](UImage* Img, UTextBlock* Txt) 
	{
		if (Img)
		{
			Img->SetBrushFromTexture(nullptr);
			Img->SetVisibility(ESlateVisibility::Hidden);
		}
		if (Txt)
		{
			Txt->SetText(FText::GetEmpty());
			Txt->SetVisibility(ESlateVisibility::Hidden);
		}
	};

	switch (SlotIndex)
	{
	case 0:
		Set(Image_SlotIcon0, Text_SlotStack0); 
		break;
	case 1:
		Set(Image_SlotIcon1, Text_SlotStack1);
		break;
	case 2:
		Set(Image_SlotIcon2, Text_SlotStack2);
		break;
	case 3:
		Set(Image_SlotIcon3, Text_SlotStack3);
		break;
	default:
		break;
	}
}

void UPlayground_QuickSlotBarWidget::SetSlotFromItem(int32 SlotIndex, UPlayground_InventoryItem* Item, int32 TotalCount)
{
	const FPlayground_ImageFragment* ImageFragment = GetFragment<FPlayground_ImageFragment>(Item, FragmentTags::IconFragment);
	UTexture2D* Icon = ImageFragment ? ImageFragment->GetIcon() : nullptr;

	//const int32 Stack = Item->GetTotalStackCount();
	const bool bShowStack = Item->IsStackable() && TotalCount > 1;

	auto Set = [&](UImage* Img, UTextBlock* Txt)
	{
		if (Img)
		{
			Img->SetBrushFromTexture(Icon);
			Img->SetVisibility(Icon ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
		if (Txt)
		{
			if (bShowStack)
			{
				Txt->SetText(FText::AsNumber(TotalCount));
				Txt->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Txt->SetText(FText::GetEmpty());
				Txt->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	};

	switch (SlotIndex)
	{
	case 0:
		Set(Image_SlotIcon0, Text_SlotStack0);
		break;
	case 1:
		Set(Image_SlotIcon1, Text_SlotStack1);
		break;
	case 2:
		Set(Image_SlotIcon2, Text_SlotStack2);
		break;
	case 3:
		Set(Image_SlotIcon3, Text_SlotStack3);
		break;
	default:
		break;
	}
}
