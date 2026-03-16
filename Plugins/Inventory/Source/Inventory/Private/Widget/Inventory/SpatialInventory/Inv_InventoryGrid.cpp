#include "Widget/Inventory/SpatialInventory/Inv_InventoryGrid.h"

#include "Inventory.h"
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
	const FInv_StackableFragment* StackableFragment = Manifest.GetFragmentOfType<FInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;
	const int32 MaxStackSize = Result.bStackable ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = Result.bStackable ? StackableFragment->GetStackCount() : 1;
	const FInv_GridFragment* GridFragment = Manifest.GetFragmentOfType<FInv_GridFragment>();
	//已检查过的GridSlot的索引
	TSet<int32> CheckedIndices;
	for (const TObjectPtr<UInv_GridSlot>& GridSlot : GridSlots)
	{
		//如果要填充的物品数量为0，则填充完毕，无需继续遍历
		if (AmountToFill == 0)break;
		//如果GridSlot已被检查过，则跳过该格子，主要用于一个物品占用多个格子时会一次性检查这些格子，避免重复检查这几个格子
		if (CheckedIndices.Contains(GridSlot->Index))continue;
		//物品所占用的所有格子是否在边界内
		if (IsInGridBounds(GridSlot->Index, GridFragment->GetGridSize()))continue;
		TSet<int32> TentativelyCheckedIndices;
		//判断该格子是否还有空间装下对应尺寸的物品
		if (!HasRoomAtIndex(GridSlot->Index, GridFragment->GetGridSize(), CheckedIndices, TentativelyCheckedIndices, Manifest.GetItemTag(),
		                    MaxStackSize))
			continue;
		const int32 AmountToFillOfThisSlot = DetermineFillAmountForSlot(Result.bStackable, MaxStackSize, AmountToFill, GridSlot);
		if (AmountToFillOfThisSlot == 0)continue;
		CheckedIndices.Append(TentativelyCheckedIndices);

		Result.TotalRoomToFill += AmountToFillOfThisSlot;
		Result.SlotAvailabilities.Emplace(FInv_SlotAvailability{
			GridSlot->Item.IsValid() ? GridSlot->UpperLeftIndex : GridSlot->Index, AmountToFillOfThisSlot, GridSlot->Item.IsValid()
		});
		AmountToFill -= AmountToFillOfThisSlot;
		Result.Remainder = AmountToFill;
	}
	return Result;
}

bool UInv_InventoryGrid::HasRoomAtIndex(int32 Index, const FIntPoint& Range2D, const TSet<int32>& CheckedIndices, TSet<int32>& TentativelyIndices,
                                        const FGameplayTag& ItemTag, int32 MaxStackSize)
{
	bool bHasRoom = true;
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Column, Range2D, [&](UInv_GridSlot* GridSlot)
	{
		if (CheckGridSlotConstraint(Index, GridSlot, CheckedIndices, ItemTag, MaxStackSize))
		{
			TentativelyIndices.Add(GridSlot->Index);
		}
		else
		{
			bHasRoom = false;
		}
	});
	return bHasRoom;
}

bool UInv_InventoryGrid::CheckGridSlotConstraint(int32 SourceIndex, UInv_GridSlot* GridSlot, const TSet<int32>& CheckedIndices,
                                                 const FGameplayTag& ItemTag, int32 MaxStackSize)
{
	if (CheckedIndices.Contains(GridSlot->Index))return false;
	if (!GridSlot->Item.IsValid())return true;
	//如果当前格子不是左上角的格子则返回false
	if (GridSlot->UpperLeftIndex != SourceIndex)return false;
	//物品是否可堆叠
	if (!GridSlot->Item->IsStackable())return false;
	//是否是同一类物品
	if (!GridSlot->Item->GetItemManifest().GetItemTag().MatchesTagExact(ItemTag))return false;
	//该格子是否装满了
	if (GridSlot->StackCount >= MaxStackSize)return false;
	return true;
}

bool UInv_InventoryGrid::IsInGridBounds(int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num())return false;
	int32 EndColumn = (StartIndex % Column) + ItemDimensions.X - 1;
	int32 EndRow = (StartIndex / Column) + ItemDimensions.Y - 1;
	return EndColumn < Column && EndRow < Row;
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
		//设置ItemWidget的各个属性
		ItemWidget->Item = Item;
		ItemWidget->GridSize = GridFragment->GetGridSize();
		ItemWidget->GridIndex = Availability.Index;
		ItemWidget->bIsStackable = Result.bStackable;
		ItemWidget->UpdateStackCount(Availability.AmountToFill);
		FSlateBrush Brush;
		Brush.SetResourceObject(IconFragment->GetIcon());
		//计算物品图片大小
		FVector2D IconSize = GridFragment->GetGridSize() * TileSize - 2 * GridFragment->GetGridPadding();
		Brush.ImageSize = IconSize;
		ItemWidget->GetImageIcon()->SetBrush(Brush);
		//将Widget添加到CanvasPanel
		CanvasPanel->AddChild(ItemWidget);
		UCanvasPanelSlot* ItemCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemWidget);
		ItemCPS->SetSize(IconSize);
		//计算绘制的位置
		const FVector2D DrawPos = UInv_WidgetUtils::GetPositionFromIndex(ItemWidget->GridIndex, Column) * TileSize + GridFragment->GetGridPadding();
		ItemCPS->SetPosition(DrawPos);
		//将Widget存储到一个容器中供以后使用
		SlottedItemWidgets.Add(Availability.Index, ItemWidget);
		UpdateGridSlot(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);
	}
}

int32 UInv_InventoryGrid::GetSlotStackAmount(const UInv_GridSlot* GridSlot) const
{
	int32 SlotStackAmount = GridSlot->StackCount;
	if (GridSlot->UpperLeftIndex != INDEX_NONE)
	{
		SlotStackAmount = GridSlots[GridSlot->UpperLeftIndex]->StackCount;
	}
	return SlotStackAmount;
}

int32 UInv_InventoryGrid::DetermineFillAmountForSlot(bool bStackable, int32 MaxStackSize, int32 AmountToFill, const UInv_GridSlot* GridSlot) const
{
	int32 RoomInSlot = MaxStackSize - GetSlotStackAmount(GridSlot);
	return bStackable ? FMath::Min(RoomInSlot, AmountToFill) : 1;
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
			const FIntPoint ColumnRow{j, i};
			GridSlot->Index = UInv_WidgetUtils::GetIndexFromPosition(ColumnRow, Column);
			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize({TileSize, TileSize});
			GridCPS->SetPosition(ColumnRow * TileSize);
			GridSlots.Add(GridSlot);
		}
	}
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

void UInv_InventoryGrid::UpdateGridSlot(UInv_InventoryItem* Item, int32 Index, bool bIsStackable, int32 StackCount)
{
	if (!GridSlots.IsValidIndex(Index))
	{
		UE_LOG(LogInventory, Error, TEXT("UpdateGridSlot Index无效"));
		return;
	}
	if (bIsStackable)
	{
		GridSlots[Index]->StackCount = StackCount;
	}
	const FInv_GridFragment* GridFragment = Item->GetItemManifest().GetFragmentOfType<FInv_GridFragment>();
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Column, GridFragment->GetGridSize(), [Index,Item](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetOccupiedTexture();
		GridSlot->UpperLeftIndex = Index;
		GridSlot->Item = Item;
		GridSlot->bAvailable = false;
	});
	GridSlots[Index]->SetOccupiedTexture();
}
