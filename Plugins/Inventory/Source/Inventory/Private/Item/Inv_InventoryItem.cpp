#include "Item/Inv_InventoryItem.h"

#include "Item/Fragment/Inv_ItemFragment.h"
#include "Net/UnrealNetwork.h"

void UInv_InventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ItemManifest);
	DOREPLIFETIME(ThisClass, TotalStackCount);
}

void UInv_InventoryItem::SetItemManifest(const FInv_ItemManifest& InItemManifest)
{
	ItemManifest = FInstancedStruct::Make<FInv_ItemManifest>(InItemManifest);
}

bool UInv_InventoryItem::IsStackable() const
{
	return GetItemManifest().GetFragmentOfType<FInv_StackableFragment>() != nullptr;
}
