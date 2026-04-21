#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Type/Inv_GridType.h"
#include "StructUtils/InstancedStruct.h"
#include "Inv_ItemManifest.generated.h"

class UInv_InventoryItem;
struct FInv_ItemFragment;

/**
 * 该结构体包含创建Item的所有必要数据
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemManifest
{
	GENERATED_BODY()

	UInv_InventoryItem* CreateItemByManifest(UObject* NewOuter) const;

	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	FGameplayTag GetItemTag() const { return ItemTag; }

	TSubclassOf<AActor> GetItemClass() const { return ItemClass; }

	template <typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentOfType() const;

	template <typename T> requires std::derived_from<T, FInv_ItemFragment>
	T* GetFragmentOfTypeMutable();

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	EInv_ItemCategory ItemCategory{EInv_ItemCategory::None};

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;
};

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
T* FInv_ItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}
