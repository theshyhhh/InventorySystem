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

UENUM(BlueprintType)
enum class EInv_TileQuadrant:uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

USTRUCT(BlueprintType)
struct FInv_TileParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory")
	FIntPoint TileCoordinate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory")
	EInv_TileQuadrant TileQuadrant{EInv_TileQuadrant::None};
};

inline bool operator==(const FInv_TileParameters& A, const FInv_TileParameters& B)
{
	return A.TileCoordinate == B.TileCoordinate
		&& A.TileIndex == B.TileIndex
		&& A.TileQuadrant == B.TileQuadrant;
}

/**
 * 移动物品时，对要放置的位置的查询结果
 */
USTRUCT()
struct FInv_SpaceQueryResult
{
	GENERATED_BODY()

	bool bHasSpace{true};

	TWeakObjectPtr<UInv_InventoryItem> Item{nullptr};

	int32 UpperLeftIndex{INDEX_NONE};
};
