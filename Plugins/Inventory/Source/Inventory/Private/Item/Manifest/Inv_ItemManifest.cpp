#include "Item/Manifest/Inv_ItemManifest.h"

#include "Item/Inv_InventoryItem.h"
#include "Item/Fragment/Inv_ItemFragment.h"
#include "Widget/Composite/Inv_CompositeBase.h"

UInv_InventoryItem* FInv_ItemManifest::CreateItemByManifest(UObject* NewOuter) const
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(NewOuter, UInv_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);
	for (TInstancedStruct<FInv_ItemFragment>& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Instantiated();
	}
	return Item;
}

void FInv_ItemManifest::AssimilateInventoryFragments(UInv_CompositeBase* Composite) const
{
	const TArray<const FInv_InventoryFragment*> InventoryFragments = GetAllFragmentsOfType<FInv_InventoryFragment>();
	for (const FInv_InventoryFragment* Fragment : InventoryFragments)
	{
		Composite->ApplyFunction([Fragment](UInv_CompositeBase* Widget)
		{
			Fragment->Assimilate(Widget);
		});
	}
}
