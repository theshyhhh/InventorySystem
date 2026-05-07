// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/SlottedItem/Inv_SlottedItemWidget.h"

#include "Components/TextBlock.h"
#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Widget/Inventory/InventoryBase/Inv_InventoryBaseWidget.h"

FReply UInv_SlottedItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlottedItemClickedDelegate.Broadcast(GridIndex, InMouseEvent);
	return FReply::Handled();
}

void UInv_SlottedItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UInv_InventoryComponent* InvComp = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (!IsValid(InvComp))return;
	if (!InvComp->InventoryMenu->HasHoverItem())
	{
		InvComp->InventoryMenu->OnItemHovered(Item.Get());
	}
}

void UInv_SlottedItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UInv_InventoryComponent* InvComp = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (!IsValid(InvComp))return;
	InvComp->InventoryMenu->OnItemUnhovered();
}

void UInv_SlottedItemWidget::UpdateStackCount(int32 Count)
{
	if (bIsStackable)
	{
		TextBlock_Count->SetVisibility(ESlateVisibility::Visible);
		TextBlock_Count->SetText(FText::AsNumber(Count));
	}
	else
	{
		TextBlock_Count->SetVisibility(ESlateVisibility::Collapsed);
	}
}
