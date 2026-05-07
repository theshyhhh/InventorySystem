#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Type/Inv_GridType.h"

#include "Inv_InventoryGrid.generated.h"

class UInv_ItemPopUpWidget;
class UInv_HoverItem;
struct FGameplayTag;
class UInv_SlottedItemWidget;
struct FInv_ItemManifest;
class UInv_ItemComponent;
class UInv_InventoryComponent;
class UCanvasPanel;
class UInv_GridSlot;
enum class EInv_GridSlotState : uint8;

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

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComp);

	//显示鼠标
	void ShowCursor();

	//隐藏鼠标
	void HideCursor();

	void SetOwningCanvasPanel(UCanvasPanel* InCanvasPanel);

	/**
	 * 丢弃悬浮中的物品
	 */
	void DropHoverItem();

	FORCEINLINE bool HasHoverItem() const;

protected:
	/**
	 * @brief 初始化库存网格组件。
	 *
	 * 该方法在组件初始化时被调用，用于执行必要的设置工作。它会首先调用基类的 `NativeOnInitialized` 方法，
	 * 然后构建库存网格，并获取与当前玩家相关的库存组件。此外，还会为库存组件添加一个动态委托，以便在向库存中添加新物品时能够响应。
	 */
	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/**
	 * 更新CurrTileParams和PrevTileParams
	 * @param CanvasPos 画布左上角的位置
	 * @param MousePos 鼠标位置
	 */
	void UpdateTileParams(const FVector2D& CanvasPos, const FVector2D& MousePos);

	void OnTileParamsUpdated();

	/**
	 * 
	 * @param CurrTileCoord 当前鼠标指向的格子坐标
	 * @param Dimension 物品占用布局
	 * @param TileQuadrant 鼠标位于当前格子的哪一象限
	 * @return 起始位置坐标
	 */
	FIntPoint CalculateStartingCoordinate(const FIntPoint& CurrTileCoord, const FIntPoint& Dimension, EInv_TileQuadrant TileQuadrant);

	/**
	 * 
	 * @param CanvasPos 画布左上角的位置
	 * @param MousePos 鼠标位置
	 * @return 返回鼠标指向的格子的坐标(列行)
	 */
	FIntPoint CalculateMouseCoordinate(const FVector2D& CanvasPos, const FVector2D& MousePos) const;

	//计算当前鼠标位于指向的格子的哪个象限
	EInv_TileQuadrant CalculateTileQuadrant(const FVector2D& CanvasPos, const FVector2D& MousePos) const;

	//检查当前指向格子的存储情况
	FInv_SpaceQueryResult CheckHoverCoordinate(const FIntPoint& Coord, const FIntPoint& Dimension);

	//返回鼠标是否这一帧离开了Canvas
	bool CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& Location);

	void HighlightGridSlots(int32 Index, const FIntPoint& Dimensions);
	void UnhighlightGridSlots(int32 Index, const FIntPoint& Dimensions);

	void ChangeHoverType(int32 Index, const FIntPoint& Dimensions, EInv_GridSlotState GridSlotState);

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest);

	bool HasRoomAtIndex(int32 Index, const FIntPoint& Range2D, const TSet<int32>& CheckedIndices, TSet<int32>& TentativelyIndices,
	                    const FGameplayTag& ItemTag, int32 MaxStackSize);

	/**
	 * 用于检测二维范围内的格子
	 * @param SourceIndex 当前检测的左上格子索引
	 * @param GridSlot 当前检测的格子
	 * @param CheckedIndices 检测过的格子
	 * @param ItemTag 要添加的物品类型标签
	 * @param MaxStackSize 当前要添加的物品的最大堆叠数
	 * @return 是否符合约束
	 */
	bool CheckGridSlotConstraint(int32 SourceIndex, const UInv_GridSlot* GridSlot, const TSet<int32>& CheckedIndices, const FGameplayTag& ItemTag,
	                             int32 MaxStackSize);

	/**
	 * 
	 * @param StartIndex 左上的起始索引
	 * @param ItemDimensions 占用格子的列行
	 * @return 所有格子是否都在边界
	 */
	bool IsInGridBounds(int32 StartIndex, const FIntPoint& ItemDimensions) const;
	void CreateAndAddItemWidget(bool bStackable, UInv_InventoryItem* Item, int32 Index, int32 StackCount);

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
	 * 获取格子内道具的数量，如果格子不是左上的格子，则获取它的左上格子种道具的数量
	 */
	int32 GetSlotStackAmount(const UInv_GridSlot* GridSlot) const;

	/***
	 * 确定该格子可添加的物品的数量
	 */
	int32 DetermineFillAmountForSlot(bool bStackable, int32 MaxStackSize, int32 AmountToFill, const UInv_GridSlot* GridSlot) const;

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

	//增加物品堆叠数目
	UFUNCTION()
	void AddItemStack(const FInv_SlotAvailabilityResult& Result);

	/**
	 * @brief 当鼠标点击道具时回调的函数
	 * 
	 * @param GridIndex 鼠标点击的道具的索引
	 * @param MouseEvent 鼠标点击的相关信息
	 */
	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	//交换物品
	void SwapWithHoverItem(UInv_InventoryItem* Item, int32 GridIndex);

	void CreateHoverItem(UInv_InventoryItem* Item, int32 GridIndex);

	void RemoveItemFromGrid(const UInv_InventoryItem* Item, int32 GridIndex);

	/**
	 * 清空HoverItem并显示鼠标
	 */
	void ClearHoverItem();

	UFUNCTION()
	void OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UUserWidget* GetVisibleCursorWidget();

	UUserWidget* GetHiddenCursorWidget();

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UUserWidget> VisibleCursorWidgetClass;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UUserWidget> HiddenCursorWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> VisibleCursorWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> HiddenCursorWidget;

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

	//都是移动物品时使用 begin

	/**
	 * 移动物品时创建的跟随鼠标移动的临时部件
	 */
	UPROPERTY()
	TObjectPtr<UInv_HoverItem> HoverItem;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInv_HoverItem> HoverItemClass;

	FInv_TileParameters CurrTileParams;

	FInv_TileParameters PrevTileParams;

	/**
	 * 移动物品时，物品将要存放的索引，随鼠标位置改变
	 */
	int32 ItemDropIndex{INDEX_NONE};

	FInv_SpaceQueryResult CurrQueryResult;

	bool bCurrMouseWithinCanvas;

	bool bPrevMouseWithinCanvas;

	//最后一次高亮的索引
	int32 LastHighlightIndex;

	//最后一次高亮的布局
	FIntPoint LastHighlightDimension;
	//都是移动物品时使用 end

	UPROPERTY()
	TObjectPtr<UInv_ItemPopUpWidget> ItemPopUpMenu;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInv_ItemPopUpWidget> ItemPopUpMenuClass;

	TWeakObjectPtr<UCanvasPanel> OwningCanvasPanel;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FVector2D ItemPopMenuOffset;

	void SplitItem(int32 SplitAmount, int32 GridIndex);

	void ConsumeItem(int32 GridIndex);

	void DropItem(int32 GridIndex);
};
