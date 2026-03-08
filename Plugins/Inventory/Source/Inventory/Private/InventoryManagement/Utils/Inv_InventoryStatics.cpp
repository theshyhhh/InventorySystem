#include "InventoryManagement/Utils/Inv_InventoryStatics.h"

#include "InventoryManagement/Component/Inv_InventoryComponent.h"

UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))return nullptr;
	UInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UInv_InventoryComponent>();
	return InventoryComponent;
}
