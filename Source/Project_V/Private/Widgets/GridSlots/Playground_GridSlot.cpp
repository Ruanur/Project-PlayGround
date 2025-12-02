// Copyright 2025. Jinsol Co. All rights reserved


#include "Widgets/GridSlots/Playground_GridSlot.h"

#include "Components/Image.h"

void UPlayground_GridSlot::PG_SetOccupiedTexture()
{
	GridSlotState = EPlayground_GridSlotState::Occupied;
	Image_GridSlot->SetBrush(Brush_Occupied);
}

void UPlayground_GridSlot::PG_SetUnoccupiedTexture()
{
	GridSlotState = EPlayground_GridSlotState::Unoccupied;
	Image_GridSlot->SetBrush(Brush_Unoccupied);
}

void UPlayground_GridSlot::PG_SetSelectedTexture()
{
	GridSlotState = EPlayground_GridSlotState::Selected;
	Image_GridSlot->SetBrush(Brush_Selected);
}

void UPlayground_GridSlot::PG_SetGrayedOutTexture()
{
	GridSlotState = EPlayground_GridSlotState::GrayedOut;
	Image_GridSlot->SetBrush(Brush_GrayedOut);
}
