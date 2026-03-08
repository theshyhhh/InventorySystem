#include "Widget/HUD/Inv_HUDWidget.h"

#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Widget/HUD/Inv_MessageWidget.h"

void UInv_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UInv_InventoryComponent* InventoryComp = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (IsValid(InventoryComp))
	{
		InventoryComp->OnInventoryNoRoomDelegate.AddDynamic(this, &UInv_HUDWidget::OnInventoryNoRoom);
	}
}

void UInv_HUDWidget::OnInventoryNoRoom()
{
	if (!IsValid(MessageWidget))return;
	MessageWidget->SetMessage(FText::FromString(TEXT("库存空间不足")));
}
