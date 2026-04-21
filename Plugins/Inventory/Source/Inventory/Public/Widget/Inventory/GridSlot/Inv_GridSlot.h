#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UInv_InventoryItem;
class UImage;
class UInv_ItemPopUpWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMouseGridSlotInteractionSignature, int32, GridIndex, const FPointerEvent&, MouseEvent);

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

	/** 该槽位存储的物品的数量 */
	int32 StackCount{0};

	/** 该槽位存储的物品的左上角槽位的索引 */
	int32 UpperLeftIndex{INDEX_NONE};

	/** 该槽位存储的道具 */
	TWeakObjectPtr<UInv_InventoryItem> Item;

	/** 该槽位是否可用 */
	bool bAvailable{true};

	void SetUnoccupiedTexture();

	void SetOccupiedTexture();

	void SetSelectedTexture();

	void SetGrayedOutTexture();

	FOnMouseGridSlotInteractionSignature OnGridSlotClickedDelegate;

	FOnMouseGridSlotInteractionSignature OnGridSlotHoveredDelegate;

	FOnMouseGridSlotInteractionSignature OnGridSlotUnhoveredDelegate;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetItemPopUpMenu(UInv_ItemPopUpWidget* InItemPopUpMenu);

	UInv_ItemPopUpWidget* GetItemPopUpMenu() const;

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

	/**
	 * @brief项目弹出菜单
	 * 该变量指向一个弱引用的UInv_ItemPopUpWidget对象，用于显示与库存网格槽位相关的弹出菜单。
	 */
	TWeakObjectPtr<UInv_ItemPopUpWidget> ItemPopUpMenu;

	UFUNCTION()
	void OnItemPopUpMenuDestruct(UUserWidget* Widget);
};
