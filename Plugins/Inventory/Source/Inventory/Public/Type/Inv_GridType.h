#pragma once

#include "Inv_GridType.generated.h"

class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_ItemCategory:uint8
{
	Equippable,
	Consumable,
	Craftable,
	None
};

USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()

	FInv_SlotAvailability()
	{
	}

	//格子索引
	int32 Index{INDEX_NONE};

	//要添加到该格子的物品数量
	int32 AmountToFill{0};

	//格子中是否有对应物品
	bool bItemAtIndex{false};
};

USTRUCT()
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FInv_SlotAvailabilityResult()
	{
	}

	//要添加到的背包中的对象
	TWeakObjectPtr<UInv_InventoryItem> Item;

	//总共可添加的数量
	int32 TotalRoomToFill{0};

	//添加后剩余的数量
	int32 Remainder{0};

	//是否可堆叠
	bool bStackable{false};

	TArray<FInv_SlotAvailability> SlotAvailabilities;
};
