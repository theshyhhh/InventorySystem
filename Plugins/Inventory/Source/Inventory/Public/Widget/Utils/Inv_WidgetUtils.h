#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inv_WidgetUtils.generated.h"

class UWidget;

UCLASS()
class INVENTORY_API UInv_WidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 
	 * @param Coordinate 行列位置信息，位置从0开始
	 * @param Column 每行有多少列
	 * @return 返回该位置的索引
	 */
	static int32 GetIndexFromCoordinate(const FIntPoint& Coordinate, const int32 Column);

	/**
	 * 
	 * @param Index 在数组中的索引
	 * @param Column 列数
	 * @return 在几列几行
	 */
	static FIntPoint GetCoordinateFromIndex(const int32 Index, const int32 Column);

	/**
	 * 
	 * @param Widget 要计算位置的Widget
	 * @return Widget左上角在视口中的位置
	 */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	static FVector2D GetWidgetPosition(UWidget* Widget);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static FVector2D GetWidgetSize(UWidget* Widget);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static bool IsWithinBounds(const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos);
};
