#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Type/Inv_GridType.h"
#include "Widget/Utils/Inv_WidgetUtils.h"
#include "Inv_InventoryStatics.generated.h"

class UInv_ItemComponent;
class UInv_InventoryComponent;


UCLASS()
class INVENTORY_API UInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UInv_InventoryComponent* GetInventoryComponent(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EInv_ItemCategory GetItemCategoryFromItemComponent(const UInv_ItemComponent* ItemComp);


	/**
	* @brief 对数组中指定的二维范围内的每个元素应用一个函数。
	*
	* 该函数会遍历给定数组中的一个二维范围，并对落在该二维范围内的每个元素
	* 应用指定的函数。遍历基于起始索引和列数进行，这两个值用于计算网格中的
	* 二维位置。
	*
	* @tparam T 数组中元素的类型。
	* @tparam FuncT 要应用到每个元素上的函数类型。
	* @param Array 要遍历的数组。
	* @param Index 数组中的起始索引，用于确定二维范围的起点。
	* @param ColumnNum 二维网格中的列数。
	* @param Range2D 一个 FIntPoint，表示要遍历的二维范围的宽度 (X) 和高度 (Y)。
	* @param Func 要应用到二维范围内每个元素上的函数。该函数应接受一个 T 类型的参数。
	*/
	template <typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array, int32 Index, int32 ColumnNum, const FIntPoint& Range2D, const FuncT& Func);
};

template <typename T, typename FuncT>
void UInv_InventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, int32 ColumnNum, const FIntPoint& Range2D, const FuncT& Func)
{
	for (int32 i = 0; i < Range2D.X; i++)
	{
		for (int32 j = 0; j < Range2D.Y; j++)
		{
			FIntPoint Coordinate = UInv_WidgetUtils::GetCoordinateFromIndex(Index, ColumnNum) + FIntPoint(i, j);
			int32 GridIndex = UInv_WidgetUtils::GetIndexFromCoordinate(Coordinate, ColumnNum);
			if (Array.IsValidIndex(GridIndex))
			{
				Func(Array[GridIndex]);
			}
		}
	}
}
