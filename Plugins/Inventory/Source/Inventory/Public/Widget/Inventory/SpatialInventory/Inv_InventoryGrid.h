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

UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComp);

private:
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest);

	/**
	 * 将道具添加到背包UI格子上对应的位置
	 * @param Result 格子插槽可用性
	 * @param Item 要添加的道具
	 */
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item);

	void ConstructGrid();

	//判断道具类型是否与格子相符
	FORCEINLINE bool MatchesCategory(const UInv_InventoryItem* Item) const;

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
