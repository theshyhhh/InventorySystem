#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SlottedItemWidget.generated.h"

class UTextBlock;
class UInv_InventoryItem;
class UImage;

UCLASS()
class INVENTORY_API UInv_SlottedItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UImage* GetImageIcon() const { return Image_Icon; }

	int32 GridIndex;

	FIntPoint GridSize;

	TWeakObjectPtr<UInv_InventoryItem> Item;

	bool bIsStackable;

	/**
	 * 更新显示的物品数量，如果bIsStackable为假则不显示，所以在调用该函数前，先设置bIsStackable
	 * @param Count 显示的数量
	 */
	void UpdateStackCount(int32 Count);

private:
	/**物品图标*/
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;

	/**用于显示物品堆叠数量，如果物品不可堆叠，则隐藏*/
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Count;
};
