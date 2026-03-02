#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inv_WidgetUtils.generated.h"

UCLASS()
class INVENTORY_API UInv_WidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 
	 * @param Position 行列位置信息，位置从0开始
	 * @param Column 每行有多少列
	 * @return 返回该位置的索引
	 */
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Column);
};
