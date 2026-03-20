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
	FIntPoint TileCoordinates;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory")
	EInv_TileQuadrant TileQuadrant{EInv_TileQuadrant::None};
};

inline bool operator==(const FInv_TileParameters& A, const FInv_TileParameters& B)
{
	return A.TileCoordinates == B.TileCoordinates
		&& A.TileIndex == B.TileIndex
		&& A.TileQuadrant == B.TileQuadrant;
}
