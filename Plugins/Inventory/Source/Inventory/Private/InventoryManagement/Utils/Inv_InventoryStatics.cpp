#include "InventoryManagement/Utils/Inv_InventoryStatics.h"

#include "Inventory.h"
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

AActor* UInv_InventoryStatics::SpawnItemByItemManifest(const UObject* WorldContextObject, const FInv_ItemManifest& ItemManifest,
                                                       const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogInventory, Error, TEXT("SpawnItemByItemManifest传入的WorldContextObject无效"))
		return nullptr;
	}
	check(ItemManifest.GetItemClass());
	AActor* SpawnedItem = WorldContextObject->GetWorld()->SpawnActor<AActor>(ItemManifest.GetItemClass(), SpawnLocation, SpawnRotation);
	if (!IsValid(SpawnedItem))
	{
		UE_LOG(LogInventory, Error, TEXT("SpawnedItem生成失败"))
		return nullptr;
	}
	UInv_ItemComponent* ItemComp = SpawnedItem->FindComponentByClass<UInv_ItemComponent>();
	check(ItemComp)
	ItemComp->SetItemManifest(ItemManifest);
	return SpawnedItem;
}
