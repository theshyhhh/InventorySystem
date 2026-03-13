#include "Widget/Inventory/GridSlot/Inv_GridSlot.h"

#include "Components/Image.h"

void UInv_GridSlot::SetUnoccupiedTexture()
{
	Image_GridSlot->SetBrush(Brush_Unoccupied);
	State = EInv_GridSlotState::Unoccupied;
}

void UInv_GridSlot::SetOccupiedTexture()
{
	Image_GridSlot->SetBrush(Brush_Occupied);
	State = EInv_GridSlotState::Occupied;
}

void UInv_GridSlot::SetSelectedTexture()
{
	Image_GridSlot->SetBrush(Brush_Selected);
	State = EInv_GridSlotState::Selected;
}

void UInv_GridSlot::SetGrayedOutTexture()
{
	Image_GridSlot->SetBrush(Brush_GrayedOut);
	State = EInv_GridSlotState::GrayedOut;
}
