#include "Widget/Inventory/SpatialInventory/Inv_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Item/Inv_InventoryItem.h"
#include "Item/Component/Inv_ItemComponent.h"
#include "Item/Fragment/Inv_ItemFragment.h"
#include "Widget/Inventory/GridSlot/Inv_GridSlot.h"
#include "Widget/Utils/Inv_WidgetUtils.h"
#include "Widget/Inventory/SlottedItem/Inv_SlottedItemWidget.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ConstructGrid();
	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAddedDelegate.AddDynamic(this, &ThisClass::UInv_InventoryGrid::AddItem);
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	if (!MatchesCategory(Item))return;
	FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);
	UE_LOG(LogTemp, Warning, TEXT("%s::AddItem"), *GetName());
	AddItemToIndices(Result, Item);
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* ItemComp)
{
	return HasRoomForItem(ItemComp->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& Manifest)
{
	FInv_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;
	FInv_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 1;
	SlotAvailability.bItemAtIndex = false;
	SlotAvailability.Index = 0;
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));

	return Result;
}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item)
{
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		//获取Item的GridFragment和ImageFragment
		const FInv_GridFragment* GridFragment = Item->GetItemManifest().GetFragmentOfType<FInv_GridFragment>();
		const FInv_IconFragment* IconFragment = Item->GetItemManifest().GetFragmentOfType<FInv_IconFragment>();
		if (!GridFragment || !IconFragment)return;
		//创建并添加一个Widget
		UInv_SlottedItemWidget* ItemWidget = CreateWidget<UInv_SlottedItemWidget>(GetOwningPlayer(), SlottedItemWidgetClass);
		ItemWidget->Item = Item;
		ItemWidget->GridSize = GridFragment->GetGridSize();
		FSlateBrush Brush;
		Brush.SetResourceObject(IconFragment->GetIcon());
		//计算物品图片大小
		FVector2D IconSize = GridFragment->GetGridSize() * TileSize - 2 * GridFragment->GetGridPadding();
		Brush.ImageSize = IconSize;
		ItemWidget->GetImageIcon()->SetBrush(Brush);
		ItemWidget->GridIndex = Availability.Index;
		//将Widget添加到CanvasPanel
		CanvasPanel->AddChild(ItemWidget);
		UCanvasPanelSlot* ItemCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemWidget);
		ItemCPS->SetSize(IconSize);
		//计算绘制的位置
		const FVector2D DrawPos = UInv_WidgetUtils::GetPositionFromIndex(ItemWidget->GridIndex, Column) * TileSize + GridFragment->GetGridPadding();
		ItemCPS->SetPosition(DrawPos);
		//将Widget存储到一个容器中供以后使用
		SlottedItemWidgets.Add(Availability.Index, ItemWidget);
	}
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

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}
