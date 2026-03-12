#include "InventoryManagement/Utils/Inv_InventoryStatics.h"

#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "Item/Component/Inv_ItemComponent.h"

UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))return nullptr;
	UInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UInv_InventoryComponent>();
	return InventoryComponent;
}

EInv_ItemCategory UInv_InventoryStatics::GetItemCategoryFromItemComponent(const UInv_ItemComponent* ItemComp)
{
	if (!IsValid(ItemComp))return EInv_ItemCategory::None;
	return ItemComp->GetItemManifest().GetItemCategory();
}
