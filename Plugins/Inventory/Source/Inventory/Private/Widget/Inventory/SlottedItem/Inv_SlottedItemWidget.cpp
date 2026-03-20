// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/SlottedItem/Inv_SlottedItemWidget.h"

#include "Components/TextBlock.h"

FReply UInv_SlottedItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlottedItemClickedDelegate.Broadcast(GridIndex, InMouseEvent);
	return FReply::Handled();
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
