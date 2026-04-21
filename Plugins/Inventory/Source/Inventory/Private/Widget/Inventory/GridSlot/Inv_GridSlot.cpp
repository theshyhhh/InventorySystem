#include "Widget/Inventory/GridSlot/Inv_GridSlot.h"

#include "Components/Image.h"
#include "Widget/ItemPopUp/Inv_ItemPopUpWidget.h"

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

void UInv_GridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnGridSlotHoveredDelegate.Broadcast(Index, InMouseEvent);
}

void UInv_GridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnGridSlotUnhoveredDelegate.Broadcast(Index, InMouseEvent);
}

FReply UInv_GridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnGridSlotClickedDelegate.Broadcast(Index, InMouseEvent);
	return FReply::Handled();
}

void UInv_GridSlot::SetItemPopUpMenu(UInv_ItemPopUpWidget* InItemPopUpMenu)
{
	ItemPopUpMenu = InItemPopUpMenu;
	InItemPopUpMenu->OnNativeDestruct.AddUObject(this, &UInv_GridSlot::OnItemPopUpMenuDestruct);
}

UInv_ItemPopUpWidget* UInv_GridSlot::GetItemPopUpMenu() const
{
	return ItemPopUpMenu.Get();
}

void UInv_GridSlot::OnItemPopUpMenuDestruct(UUserWidget* Widget)
{
	ItemPopUpMenu = nullptr;
}
