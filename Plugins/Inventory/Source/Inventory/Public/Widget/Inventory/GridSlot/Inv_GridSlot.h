#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;

/**
 * @brief 库存网格槽位组件
 * 该组件用于表示库存中的一个网格槽位，可以显示不同的状态（未占用、已占用、选中、灰化）。
 */
enum class EInv_GridSlotState : uint8
{
	/** 未占用状态 */
	Unoccupied,
	/** 已占用状态 */
	Occupied,
	/** 选中状态 */
	Selected,
	/** 灰化状态 */
	GrayedOut
};

/**
 * @brief 库存网格槽位用户界面组件
 * 该组件继承自 UUserWidget，用于在用户界面中显示库存网格槽位，并根据状态显示不同的图像。
 */
UCLASS()
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 网格槽位索引 */
	int32 Index;

	void SetUnoccupiedTexture();

	void SetOccupiedTexture();

	void SetSelectedTexture();

	void SetGrayedOutTexture();

private:
	/** 绑定到蓝图的网格槽位图像 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	/** 未占用状态下的Brush */
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	/** 已占用状态下的Brush */
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	/** 选中状态下的Brush */
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	/** 灰化状态下的Brush */
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;

	/**
	 * @brief 网格槽位的状态枚举
	 * 用于表示库存网格槽位的不同状态，包括未占用、已占用、选中和灰化。
	 */
	EInv_GridSlotState State = EInv_GridSlotState::Unoccupied;
};
