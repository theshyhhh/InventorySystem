#pragma once

#include "CoreMinimal.h"
#include "Widget/Inventory/InventoryBase/Inv_InventoryBaseWidget.h"
#include "Inv_SpatialInventoryWidget.generated.h"

class UCanvasPanel;
class UButton;
class UWidgetSwitcher;
class UInv_InventoryGrid;

UCLASS()
class INVENTORY_API UInv_SpatialInventoryWidget : public UInv_InventoryBaseWidget
{
	GENERATED_BODY()

public:
	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComp) override;

protected:
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> GridSwitcher;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Equippable;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Consumable;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Craftable;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Equippable;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Consumable;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Craftable;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UFUNCTION()
	void ShowEquippable();

	UFUNCTION()
	void ShowConsumable();

	UFUNCTION()
	void ShowCraftable();

	/**设置要激活的库存格子
	 * @param InventoryGrid 要激活的库存格子
	 * @param Button 要禁用的按键
	 */
	void SetActiveGrid(UInv_InventoryGrid* InventoryGrid, UButton* Button);

	TWeakObjectPtr<UInv_InventoryGrid> ActiveGrid;
};
