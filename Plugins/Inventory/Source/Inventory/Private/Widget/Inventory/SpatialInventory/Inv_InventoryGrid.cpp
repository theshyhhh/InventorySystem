#include "Widget/Inventory/SpatialInventory/Inv_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widget/Inventory/GridSlot/Inv_GridSlot.h"
#include "Widget/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ConstructGrid();
}

void UInv_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Row * Column);
	for (int32 i = 0; i < Row; i++)
	{
		for (int32 j = 0; j < Column; j++)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);
			const FIntPoint RowColumn{i, j};
			GridSlot->Index = UInv_WidgetUtils::GetIndexFromPosition(RowColumn, Column);
			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize({TileSize, TileSize});
			const FIntPoint Position{j, i};
			GridCPS->SetPosition(Position * TileSize);
			GridSlots.Add(GridSlot);
		}
	}
}
