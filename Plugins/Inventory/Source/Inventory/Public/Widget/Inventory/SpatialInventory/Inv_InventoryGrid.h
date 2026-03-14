#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Type/Inv_GridType.h"

#include "Inv_InventoryGrid.generated.h"

class UInv_SlottedItemWidget;
struct FInv_ItemManifest;
class UInv_ItemComponent;
class UInv_InventoryComponent;
class UCanvasPanel;
class UInv_GridSlot;

/**
 * @class UInv_InventoryGrid
 * @brief 用于管理库存网格的用户界面组件。
 *
 * 该类继承自 `UUserWidget`，并提供了构建和管理库存网格的功能。它支持添加物品、检查是否有足够的空间存放新物品等操作。
 * 库存网格可以根据配置的行数和列数动态生成，并且可以指定允许存储的物品类别。
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	/**
	 * @brief 初始化库存网格组件。
	 *
	 * 该方法在组件初始化时被调用，用于执行必要的设置工作。它会首先调用基类的 `NativeOnInitialized` 方法，
	 * 然后构建库存网格，并获取与当前玩家相关的库存组件。此外，还会为库存组件添加一个动态委托，以便在向库存中添加新物品时能够响应。
	 */
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComp);

private:
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest);

	/**
	 * @brief 根据结果中提供的可用槽位将物品添加到库存网格中。
	 *
	 * 该方法会处理槽位可用性结果，并将给定物品添加到每一个可用槽位。
	 * 它会为该物品创建一个控件，设置其属性，并将其放置在 Canvas Panel 上。
	 *
	 * @param Result 槽位可用性结果，包含物品可以放置的位置相关信息。
	 * @param Item 要添加到网格中的库存物品。
	 */
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item);

	/**
	 * @brief 构建库存网格。
	 *
	 * 该方法负责根据配置的行数和列数动态生成库存网格。它会创建指定数量的 `UInv_GridSlot` 实例，并将它们添加到 `CanvasPanel` 上，
	 * 同时设置每个格子的位置和大小，以便正确显示在界面上。此外，还会为每个创建的格子保留一个引用，以供后续操作使用。
	 */
	void ConstructGrid();

	//判断道具类型是否与格子相符
	FORCEINLINE bool MatchesCategory(const UInv_InventoryItem* Item) const;

	/**
	 * 当物品添加时，更新被占用的网格的样式
	 */
	void UpdateGridSlot(UInv_InventoryItem* Item, int32 Index, bool bIsStackable, int32 StackCount);

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	//格子的边长
	UPROPERTY(EditAnywhere, Category="Inventory")
	float TileSize;

	//行数
	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 Row;

	//列数
	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 Column;

	//存储的道具类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess=true))
	EInv_ItemCategory ItemCategory = EInv_ItemCategory::None;

	//要创建的物品Widget
	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInv_SlottedItemWidget> SlottedItemWidgetClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UInv_SlottedItemWidget>> SlottedItemWidgets;
};
