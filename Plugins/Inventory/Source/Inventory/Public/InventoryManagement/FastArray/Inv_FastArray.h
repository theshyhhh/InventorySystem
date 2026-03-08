#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "Inv_FastArray.generated.h"

class UInv_ItemComponent;
class UInv_InventoryItem;
class UInv_InventoryComponent;
struct FInv_InventoryFastArraySerializer;

/**库存系统的单一条目*/
USTRUCT(BlueprintType)
struct FInv_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInv_InventoryEntry()
	{
	}

private:
	friend UInv_InventoryComponent;
	friend FInv_InventoryFastArraySerializer;

	UPROPERTY()
	TObjectPtr<UInv_InventoryItem> Item = nullptr;
};

/**库存系统道具的清单*/
USTRUCT()
struct FInv_InventoryFastArraySerializer : public FFastArraySerializer
{
	GENERATED_BODY()

	FInv_InventoryFastArraySerializer() : OwningComponent(nullptr)
	{
	}

	FInv_InventoryFastArraySerializer(UInv_InventoryComponent* InOwningComponent) : OwningComponent(InOwningComponent)
	{
	}

	//FFastArraySerializer Contract Begin

	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);

	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	//FFastArraySerializer Contract End

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FInv_InventoryEntry, FInv_InventoryFastArraySerializer>(Entries, DeltaParams, *this);
	}

	UInv_InventoryItem* AddEntry(UInv_ItemComponent* ItemComponent);

	UInv_InventoryItem* AddEntry(UInv_InventoryItem* Item);

	void RemoveEntry(UInv_InventoryItem* Item);

	TArray<UInv_InventoryItem*> GetAllItems();

private:
	friend UInv_InventoryComponent;

	//道具的复制列表
	UPROPERTY()
	TArray<FInv_InventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UInv_InventoryComponent> OwningComponent;
};

template <>
struct TStructOpsTypeTraits<FInv_InventoryFastArraySerializer> : TStructOpsTypeTraitsBase2<FInv_InventoryFastArraySerializer>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};
