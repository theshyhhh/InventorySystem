// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ItemDescription/Inv_ItemDescriptionWidget.h"

#include "Components/SizeBox.h"

FVector2D UInv_ItemDescriptionWidget::GetSizeBox() const
{
	return SizeBox->GetDesiredSize();
}
