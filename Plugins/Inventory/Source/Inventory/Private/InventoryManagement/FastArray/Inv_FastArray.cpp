#include "InventoryManagement/FastArray/Inv_FastArray.h"

#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "Item/Inv_InventoryItem.h"


void FInv_InventoryFastArraySerializer::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	if (!IsValid(OwningComponent))return;
	for (const auto& Index : RemovedIndices)
	{
		OwningComponent->OnItemRemovedDelegate.Broadcast(Entries[Index].Item);
	}
}

void FInv_InventoryFastArraySerializer::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	if (!IsValid(OwningComponent))return;
	for (const auto& Index : AddedIndices)
	{
		OwningComponent->OnItemAddedDelegate.Broadcast(Entries[Index].Item);
	}
}

UInv_InventoryItem* FInv_InventoryFastArraySerializer::AddEntry(UInv_ItemComponent* ItemComponent)
{
	return nullptr;
}

UInv_InventoryItem* FInv_InventoryFastArraySerializer::AddEntry(UInv_InventoryItem* Item)
{
	check(OwningComponent)
	AActor* OwningActor = OwningComponent->GetOwner();
	check(OwningActor->HasAuthority())
	FInv_InventoryEntry& Entry = Entries.AddDefaulted_GetRef();
	Entry.Item = Item;
	MarkItemDirty(Entry);
	return Item;
}

void FInv_InventoryFastArraySerializer::RemoveEntry(UInv_InventoryItem* Item)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FInv_InventoryEntry& Entry = *EntryIt;
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<UInv_InventoryItem*> FInv_InventoryFastArraySerializer::GetAllItems()
{
	TArray<UInv_InventoryItem*> Items;
	Items.Reserve(Entries.Num());
	for (const FInv_InventoryEntry& Entry : Entries)
	{
		if (!IsValid(Entry.Item))continue;
		Items.Add(Entry.Item);
	}
	return Items;
}
