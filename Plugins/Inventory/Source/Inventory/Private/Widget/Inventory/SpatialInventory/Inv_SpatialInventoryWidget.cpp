#include "Widget/Inventory/SpatialInventory/Inv_SpatialInventoryWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Widget/Inventory/SpatialInventory/Inv_InventoryGrid.h"

void UInv_SpatialInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button_Equippable->OnClicked.AddDynamic(this, &UInv_SpatialInventoryWidget::ShowEquippable);
	Button_Consumable->OnClicked.AddDynamic(this, &UInv_SpatialInventoryWidget::ShowConsumable);
	Button_Craftable->OnClicked.AddDynamic(this, &UInv_SpatialInventoryWidget::ShowCraftable);
	ShowEquippable();
}

void UInv_SpatialInventoryWidget::ShowEquippable()
{
	SetActiveGrid(Grid_Equippable, Button_Equippable);
}

void UInv_SpatialInventoryWidget::ShowConsumable()
{
	SetActiveGrid(Grid_Consumable, Button_Consumable);
}

void UInv_SpatialInventoryWidget::ShowCraftable()
{
	SetActiveGrid(Grid_Craftable, Button_Craftable);
}

void UInv_SpatialInventoryWidget::SetActiveGrid(UInv_InventoryGrid* InventoryGrid, UButton* Button)
{
	Button_Equippable->SetIsEnabled(true);
	Button_Consumable->SetIsEnabled(true);
	Button_Craftable->SetIsEnabled(true);
	Button->SetIsEnabled(false);
	GridSwitcher->SetActiveWidget(InventoryGrid);
}
