#include "Widget/Inventory/SpatialInventory/Inv_SpatialInventoryWidget.h"

#include "Inventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Item/Inv_InventoryItem.h"
#include "Widget/Inventory/SpatialInventory/Inv_InventoryGrid.h"
#include "Widget/ItemDescription/Inv_ItemDescriptionWidget.h"

void UInv_SpatialInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button_Equippable->OnClicked.AddDynamic(this, &UInv_SpatialInventoryWidget::ShowEquippable);
	Button_Consumable->OnClicked.AddDynamic(this, &UInv_SpatialInventoryWidget::ShowConsumable);
	Button_Craftable->OnClicked.AddDynamic(this, &UInv_SpatialInventoryWidget::ShowCraftable);
	Grid_Equippable->SetOwningCanvasPanel(CanvasPanel);
	Grid_Consumable->SetOwningCanvasPanel(CanvasPanel);
	Grid_Craftable->SetOwningCanvasPanel(CanvasPanel);
	ShowEquippable();
}

FReply UInv_SpatialInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ActiveGrid->DropHoverItem();
	return FReply::Handled();
}

void UInv_SpatialInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!IsValid(ItemDescriptionWidget))return;

	UCanvasPanelSlot* DescriptionSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemDescriptionWidget);
	if (!IsValid(DescriptionSlot))return;

	DescriptionSlot->SetSize(ItemDescriptionWidget->GetSizeBox());

	DescriptionSlot->SetPosition(UInv_WidgetUtils::GetClampedWidgetPosition(UInv_WidgetUtils::GetWidgetSize(CanvasPanel),
	                                                                        ItemDescriptionWidget->GetSizeBox(),
	                                                                        UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer())));
}

FInv_SlotAvailabilityResult UInv_SpatialInventoryWidget::HasRoomForItem(UInv_ItemComponent* ItemComp)
{
	switch (UInv_InventoryStatics::GetItemCategoryFromItemComponent(ItemComp))
	{
	case EInv_ItemCategory::Equippable:
		return Grid_Equippable->HasRoomForItem(ItemComp);
	case EInv_ItemCategory::Consumable:
		return Grid_Consumable->HasRoomForItem(ItemComp);
	case EInv_ItemCategory::Craftable:
		return Grid_Craftable->HasRoomForItem(ItemComp);
	default:
		UE_LOG(LogInventory, Error, TEXT("ItemComp没有有效的ItemCategory"))
		return FInv_SlotAvailabilityResult();
	}
}

void UInv_SpatialInventoryWidget::OnItemHovered(UInv_InventoryItem* Item)
{
	if (!IsValid(ItemDescriptionWidget))
	{
		ItemDescriptionWidget = CreateWidget<UInv_ItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
		CanvasPanel->AddChild(ItemDescriptionWidget);
		ItemDescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(ItemDescriptionTimer);
	const FInv_ItemManifest& Manifest = Item->GetItemManifest();
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this,&Manifest]()
	{
		Manifest.AssimilateInventoryFragments(ItemDescriptionWidget);
		ItemDescriptionWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	});
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(ItemDescriptionTimer, TimerDelegate, ShowItemDescriptionDelayTime, false);
}

void UInv_SpatialInventoryWidget::OnItemUnhovered()
{
	if (!IsValid(ItemDescriptionWidget))return;
	ItemDescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(ItemDescriptionTimer);
}

bool UInv_SpatialInventoryWidget::HasHoverItem()
{
	return Grid_Consumable->HasHoverItem() || Grid_Equippable->HasHoverItem() || Grid_Craftable->HasHoverItem();
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
	ActiveGrid = InventoryGrid;
	InventoryGrid->ShowCursor();
}
