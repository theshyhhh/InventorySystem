#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HoverItem.generated.h"

class UTextBlock;
class UInv_InventoryItem;
class UImage;
/**
 * 库存中物品被点击时生成并跟随鼠标移动的部件
 */
UCLASS()
class INVENTORY_API UInv_HoverItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetImageBrush(const FSlateBrush& Brush);

	void UpdateStackCount(int32 Count);

	int32 GetStackCount() const { return StackCount; }

	FGameplayTag GetItemTag() const;

	bool bIsStackable;

	int32 PreviousGridIndex;

	FIntPoint GridDimensions;

	TWeakObjectPtr<UInv_InventoryItem> Item;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StackCount;

	int32 StackCount;
};
