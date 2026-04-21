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
#include "Widget/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widget/Utils/Inv_WidgetUtils.h"
#include "Widget/Inventory/SlottedItem/Inv_SlottedItemWidget.h"
#include "Widget/ItemPopUp/Inv_ItemPopUpWidget.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ConstructGrid();
	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAddedDelegate.AddDynamic(this, &ThisClass::UInv_InventoryGrid::AddItem);
	InventoryComponent->OnItemStackChangeDelegate.AddDynamic(this, &ThisClass::AddItemStack);
}

void UInv_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	FVector2D CanvasPos = UInv_WidgetUtils::GetWidgetPosition(CanvasPanel);
	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *CanvasPos.ToString()));
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *MousePos.ToString()));
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *UInv_WidgetUtils::GetWidgetSize(CanvasPanel).ToString()));
	if (CursorExitedCanvas(CanvasPos, UInv_WidgetUtils::GetWidgetSize(CanvasPanel), MousePos))
	{
		return;
	}
	UpdateTileParams(CanvasPos, MousePos);
}

void UInv_InventoryGrid::UpdateTileParams(const FVector2D& CanvasPos, const FVector2D& MousePos)
{
	if (!bCurrMouseWithinCanvas)return;
	//计算鼠标指向哪个格子的哪个象限
	FIntPoint MouseCoordinate = CalculateMouseCoordinate(CanvasPos, MousePos);
	PrevTileParams = CurrTileParams;
	CurrTileParams.TileCoordinate = MouseCoordinate;
	CurrTileParams.TileIndex = UInv_WidgetUtils::GetIndexFromCoordinate(MouseCoordinate, Column);
	CurrTileParams.TileQuadrant = CalculateTileQuadrant(CanvasPos, MousePos);
	//控制高光和取消高光
	OnTileParamsUpdated();
}

void UInv_InventoryGrid::OnTileParamsUpdated()
{
	if (!IsValid(HoverItem))return;
	//当前移动物品布局下的起始坐标（列行）
	FIntPoint StartingCoord = CalculateStartingCoordinate(CurrTileParams.TileCoordinate, HoverItem->GridDimensions, CurrTileParams.TileQuadrant);
	//起始坐标下的索引
	ItemDropIndex = UInv_WidgetUtils::GetIndexFromCoordinate(StartingCoord, Column);
	CurrQueryResult = CheckHoverCoordinate(StartingCoord, HoverItem->GridDimensions);
	if (CurrQueryResult.bHasSpace)
	{
		HighlightGridSlots(ItemDropIndex, HoverItem->GridDimensions);
		return;
	}
	UnhighlightGridSlots(LastHighlightIndex, LastHighlightDimension);
	if (CurrQueryResult.Item.IsValid() && GridSlots.IsValidIndex(CurrQueryResult.UpperLeftIndex))
	{
		const FInv_GridFragment* GridFragment = CurrQueryResult.Item->GetItemManifest().GetFragmentOfType<FInv_GridFragment>();
		ChangeHoverType(CurrQueryResult.UpperLeftIndex, GridFragment->GetGridSize(), EInv_GridSlotState::GrayedOut);
	}
}

FIntPoint UInv_InventoryGrid::CalculateStartingCoordinate(const FIntPoint& CurrTileCoord, const FIntPoint& Dimension, EInv_TileQuadrant TileQuadrant)
{
	//判断物品布局宽高是否为偶数
	const int32 HasEvenWidth = Dimension.X % 2 == 0;
	const int32 HasEvenHeight = Dimension.Y % 2 == 0;
	FIntPoint StartingCoord;
	switch (TileQuadrant)
	{
	case EInv_TileQuadrant::TopLeft:
		StartingCoord.X = CurrTileCoord.X - FMath::FloorToInt32(Dimension.X * 0.5f);
		StartingCoord.Y = CurrTileCoord.Y - FMath::FloorToInt32(Dimension.Y * 0.5f);
		break;
	case EInv_TileQuadrant::TopRight:
		StartingCoord.X = CurrTileCoord.X - FMath::FloorToInt32(Dimension.X * 0.5f) + HasEvenWidth;
		StartingCoord.Y = CurrTileCoord.Y - FMath::FloorToInt32(Dimension.Y * 0.5f);
		break;
	case EInv_TileQuadrant::BottomLeft:
		StartingCoord.X = CurrTileCoord.X - FMath::FloorToInt32(Dimension.X * 0.5f);
		StartingCoord.Y = CurrTileCoord.Y - FMath::FloorToInt32(Dimension.Y * 0.5f) + HasEvenHeight;
		break;
	case EInv_TileQuadrant::BottomRight:
		StartingCoord.X = CurrTileCoord.X - FMath::FloorToInt32(Dimension.X * 0.5f) + HasEvenWidth;
		StartingCoord.Y = CurrTileCoord.Y - FMath::FloorToInt32(Dimension.Y * 0.5f) + HasEvenHeight;
		break;
	default:
		break;
	}
	return StartingCoord;
}

FIntPoint UInv_InventoryGrid::CalculateMouseCoordinate(const FVector2D& CanvasPos, const FVector2D& MousePos) const
{
	return FIntPoint
	{
		FMath::FloorToInt32((MousePos.X - CanvasPos.X) / TileSize),
		FMath::FloorToInt32((MousePos.Y - CanvasPos.Y) / TileSize)
	};
}

EInv_TileQuadrant UInv_InventoryGrid::CalculateTileQuadrant(const FVector2D& CanvasPos, const FVector2D& MousePos) const
{
	bool bIsTop = FMath::Fmod(MousePos.Y - CanvasPos.Y, TileSize) < TileSize / 2;
	bool bIsLeft = FMath::Fmod(MousePos.X - CanvasPos.X, TileSize) < TileSize / 2;
	if (bIsTop && bIsLeft)return EInv_TileQuadrant::TopLeft;
	if (bIsTop && !bIsLeft)return EInv_TileQuadrant::TopRight;
	if (!bIsTop && bIsLeft)return EInv_TileQuadrant::BottomLeft;
	return EInv_TileQuadrant::BottomRight;
}

FInv_SpaceQueryResult UInv_InventoryGrid::CheckHoverCoordinate(const FIntPoint& Coord, const FIntPoint& Dimension)
{
	FInv_SpaceQueryResult Result;
	const int32 Index = UInv_WidgetUtils::GetIndexFromCoordinate(Coord, Column);
	//是否物品在背包边界内
	if (!IsInGridBounds(Index, Dimension))
	{
		Result.bHasSpace = false;
		return Result;
	}
	//判断要占用的格子中是否已有物品，且是不是同一件物品
	TSet<int32> OccupiedUpperLeftIndices;
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Column, Dimension, [&](const UInv_GridSlot* GridSlot)
	{
		if (GridSlot->Item.IsValid())
		{
			OccupiedUpperLeftIndices.Add(GridSlot->UpperLeftIndex);
			Result.bHasSpace = false;
		}
	});
	//如果是同一件物品，是否与移动中的物品可交换位置
	if (OccupiedUpperLeftIndices.Num() == 1)
	{
		Result.UpperLeftIndex = *OccupiedUpperLeftIndices.CreateConstIterator();
		Result.Item = GridSlots[Result.UpperLeftIndex]->Item;
	}
	return Result;
}

bool UInv_InventoryGrid::CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& Location)
{
	bPrevMouseWithinCanvas = bCurrMouseWithinCanvas;
	bCurrMouseWithinCanvas = UInv_WidgetUtils::IsWithinBounds(BoundaryPos, BoundarySize, Location);
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("%d"), bCurrMouseWithinCanvas));
	if (!bCurrMouseWithinCanvas && bPrevMouseWithinCanvas)
	{
		UnhighlightGridSlots(LastHighlightIndex, LastHighlightDimension);
		return true;
	}
	return false;
}

void UInv_InventoryGrid::HighlightGridSlots(int32 Index, const FIntPoint& Dimensions)
{
	if (!bCurrMouseWithinCanvas)return;
	UnhighlightGridSlots(LastHighlightIndex, LastHighlightDimension);
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Column, Dimensions, [](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetOccupiedTexture();
	});
	LastHighlightIndex = Index;
	LastHighlightDimension = Dimensions;
}

void UInv_InventoryGrid::UnhighlightGridSlots(int32 Index, const FIntPoint& Dimensions)
{
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Column, Dimensions, [](UInv_GridSlot* GridSlot)
	{
		if (GridSlot->bAvailable)
		{
			GridSlot->SetUnoccupiedTexture();
		}
		else
		{
			GridSlot->SetOccupiedTexture();
		}
	});
}

void UInv_InventoryGrid::ChangeHoverType(int32 Index, const FIntPoint& Dimensions, EInv_GridSlotState GridSlotState)
{
	UnhighlightGridSlots(LastHighlightIndex, LastHighlightDimension);
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Column, Dimensions, [GridSlotState](UInv_GridSlot* GridSlot)
	{
		switch (GridSlotState)
		{
		case EInv_GridSlotState::Occupied:
			GridSlot->SetOccupiedTexture();
			break;
		case EInv_GridSlotState::Unoccupied:
			GridSlot->SetUnoccupiedTexture();
			break;
		case EInv_GridSlotState::GrayedOut:
			GridSlot->SetGrayedOutTexture();
			break;
		case EInv_GridSlotState::Selected:
			GridSlot->SetSelectedTexture();
			break;
		default:
			break;
		}
	});
	LastHighlightIndex = Index;
	LastHighlightDimension = Dimensions;
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
		if (!IsInGridBounds(GridSlot->Index, GridFragment->GetGridSize()))continue;
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
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Column, Range2D, [&](const UInv_GridSlot* GridSlot)
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

bool UInv_InventoryGrid::CheckGridSlotConstraint(int32 SourceIndex, const UInv_GridSlot* GridSlot, const TSet<int32>& CheckedIndices,
                                                 const FGameplayTag& ItemTag, int32 MaxStackSize)
{
	if (CheckedIndices.Contains(GridSlot->Index))return false;
	if (!GridSlot->Item.IsValid())return true;
	//如果当前子格子的左上格子不是当前检测的格子则返回false
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

void UInv_InventoryGrid::CreateAndAddItemWidget(bool bStackable, UInv_InventoryItem* Item, int32 Index, int32 StackCount)
{
	//获取Item的GridFragment和ImageFragment
	const FInv_GridFragment* GridFragment = Item->GetItemManifest().GetFragmentOfType<FInv_GridFragment>();
	const FInv_IconFragment* IconFragment = Item->GetItemManifest().GetFragmentOfType<FInv_IconFragment>();
	checkf(GridFragment&&GridFragment, TEXT("物品缺少格子布局属性和图标属性"))
	//创建并添加一个Widget
	UInv_SlottedItemWidget* ItemWidget = CreateWidget<UInv_SlottedItemWidget>(GetOwningPlayer(), SlottedItemWidgetClass);
	//设置ItemWidget的各个属性
	ItemWidget->Item = Item;
	ItemWidget->GridSize = GridFragment->GetGridSize();
	ItemWidget->GridIndex = Index;
	ItemWidget->bIsStackable = bStackable;
	ItemWidget->UpdateStackCount(StackCount);
	ItemWidget->OnSlottedItemClickedDelegate.AddDynamic(this, &ThisClass::OnSlottedItemClicked);

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
	const FVector2D DrawPos = UInv_WidgetUtils::GetCoordinateFromIndex(ItemWidget->GridIndex, Column) * TileSize + GridFragment->GetGridPadding();
	ItemCPS->SetPosition(DrawPos);
	//将Widget存储到一个容器中供以后使用
	SlottedItemWidgets.Add(Index, ItemWidget);
}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item)
{
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		CreateAndAddItemWidget(Result.bStackable, Item, Availability.Index, Availability.AmountToFill);
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
			GridSlot->Index = UInv_WidgetUtils::GetIndexFromCoordinate(ColumnRow, Column);
			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize({TileSize, TileSize});
			GridCPS->SetPosition(ColumnRow * TileSize);
			GridSlots.Add(GridSlot);
			GridSlot->OnGridSlotClickedDelegate.AddDynamic(this, &UInv_InventoryGrid::OnGridSlotClicked);
			GridSlot->OnGridSlotHoveredDelegate.AddDynamic(this, &UInv_InventoryGrid::OnGridSlotHovered);
			GridSlot->OnGridSlotUnhoveredDelegate.AddDynamic(this, &UInv_InventoryGrid::OnGridSlotUnhovered);
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

void UInv_InventoryGrid::AddItemStack(const FInv_SlotAvailabilityResult& Result)
{
	if (!MatchesCategory(Result.Item.Get()))return;
	for (const FInv_SlotAvailability& Availability : Result.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex)
		{
			UInv_SlottedItemWidget* SlottedItemWidget = SlottedItemWidgets.FindChecked(Availability.Index);
			GridSlots[Availability.Index]->StackCount += Availability.AmountToFill;
			SlottedItemWidget->UpdateStackCount(GridSlots[Availability.Index]->StackCount);
		}
		else
		{
			CreateAndAddItemWidget(Result.bStackable, Result.Item.Get(), Availability.Index, Availability.AmountToFill);
			UpdateGridSlot(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
		}
	}
}

void UInv_InventoryGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	checkf(GridSlots.IsValidIndex(GridIndex), TEXT("OnSlottedItemClicked传入的索引值无效"));
	if (!IsValid(HoverItem) && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		//创建悬浮部件
		CreateHoverItem(GridSlots[GridIndex]->Item.Get(), GridIndex);
		//从背包UI中移除道具
		RemoveItemFromGrid(GridSlots[GridIndex]->Item.Get(), GridIndex);
		return;
	}
	//如果是右键点击SlottedItem且当前没有HoverItem和ItemPopUpMenu则创建ItemPopUpMenu
	if (!IsValid(HoverItem) && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton && !IsValid(GridSlots[GridIndex]->GetItemPopUpMenu()))
	{
		ItemPopUpMenu = CreateWidget<UInv_ItemPopUpWidget>(this, ItemPopUpMenuClass);
		ItemPopUpMenu->GridIndex = GridIndex;
		GridSlots[GridIndex]->SetItemPopUpMenu(ItemPopUpMenu);
		UCanvasPanelSlot* CanvasPanelSlot = OwningCanvasPanel->AddChildToCanvas(ItemPopUpMenu);
		CanvasPanelSlot->SetPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()) - ItemPopMenuOffset);
		CanvasPanelSlot->SetSize(ItemPopUpMenu->GetRootSizeBoxSize());
		if (GridSlots[GridIndex]->Item->IsStackable() && GridSlots[GridIndex]->StackCount > 1)
		{
			ItemPopUpMenu->SetSliderMaxValue(GridSlots[GridIndex]->StackCount - 1);
			ItemPopUpMenu->OnSplitButtonClickedDelegate.BindUObject(this, &UInv_InventoryGrid::SplitItem);
		}
		else
		{
			ItemPopUpMenu->CollapseSplitButton();
		}
		ItemPopUpMenu->OnDropButtonClickedDelegate.BindUObject(this, &UInv_InventoryGrid::DropItem);
		if (!GridSlots[GridIndex]->Item->IsConsumable())
		{
			ItemPopUpMenu->CollapseConsumeButton();
		}
		else
		{
			ItemPopUpMenu->OnConsumeButtonClickedDelegate.BindUObject(this, &UInv_InventoryGrid::ConsumeItem);
		}
		return;
	}
	if (IsValid(HoverItem) && HoverItem->Item->GetItemManifest().GetItemTag().MatchesTagExact(
			GridSlots[GridIndex]->Item->GetItemManifest().GetItemTag())
		&& HoverItem->Item->IsStackable())
	{
		//被点击的槽位中的物品数量
		const int32 ClickedStackCount = GridSlots[GridIndex]->StackCount;
		const FInv_StackableFragment* StackableFragment = GridSlots[GridIndex]->Item->GetItemManifest().GetFragmentOfType<FInv_StackableFragment>();
		//该类物品最大物品数量
		const int32 MaxStackCount = StackableFragment->GetMaxStackSize();
		//被点击的槽位中还有多少空间
		const int32 RoomInClickedSlot = MaxStackCount - ClickedStackCount;
		//被移动的物品的数量
		const int32 HoverStackCount = HoverItem->GetStackCount();
		if (RoomInClickedSlot == 0 && HoverStackCount < MaxStackCount)
		{
			//交换物品的数量
			GridSlots[GridIndex]->StackCount = HoverStackCount;
			SlottedItemWidgets[GridIndex]->UpdateStackCount(HoverStackCount);
			HoverItem->UpdateStackCount(ClickedStackCount);
		}
		else if (RoomInClickedSlot >= HoverStackCount)
		{
			//如果空间足够，将移动中的物品添加到槽位中
			GridSlots[GridIndex]->StackCount += HoverStackCount;
			SlottedItemWidgets[GridIndex]->UpdateStackCount(GridSlots[GridIndex]->StackCount);
			ClearHoverItem();

			ChangeHoverType(GridIndex, GridSlots[GridIndex]->Item->GetItemManifest().GetFragmentOfType<FInv_GridFragment>()->GetGridSize(),
			                EInv_GridSlotState::Occupied);
		}
		else if (RoomInClickedSlot > 0 && RoomInClickedSlot < HoverStackCount)
		{
			//如果有空间，但空间不足，把能添加的物品添加到槽位中
			GridSlots[GridIndex]->StackCount = MaxStackCount;
			SlottedItemWidgets[GridIndex]->UpdateStackCount(MaxStackCount);
			HoverItem->UpdateStackCount(HoverStackCount - RoomInClickedSlot);
		}
		return;
	}
	SwapWithHoverItem(GridSlots[GridIndex]->Item.Get(), GridIndex);
}

void UInv_InventoryGrid::SwapWithHoverItem(UInv_InventoryItem* Item, int32 GridIndex)
{
	if (!IsValid(HoverItem))return;
	UInv_InventoryItem* TempItem = HoverItem->Item.Get();
	const int32 TempStackCount = HoverItem->GetStackCount();
	const bool bTempIsStackable = HoverItem->bIsStackable;
	CreateHoverItem(Item, GridIndex);
	RemoveItemFromGrid(Item, GridIndex);
	CreateAndAddItemWidget(bTempIsStackable, TempItem, ItemDropIndex, TempStackCount);
	UpdateGridSlot(TempItem, ItemDropIndex, bTempIsStackable, TempStackCount);
}

void UInv_InventoryGrid::CreateHoverItem(UInv_InventoryItem* Item, int32 GridIndex)
{
	HoverItem = CreateWidget<UInv_HoverItem>(GetOwningPlayer(), HoverItemClass);
	//获取Item的GridFragment和ImageFragment
	const FInv_GridFragment* GridFragment = Item->GetItemManifest().GetFragmentOfType<FInv_GridFragment>();
	const FInv_IconFragment* IconFragment = Item->GetItemManifest().GetFragmentOfType<FInv_IconFragment>();
	checkf(GridFragment&&GridFragment, TEXT("物品缺少格子布局属性和图标属性"))
	//计算物品图片大小
	FVector2D IconSize = GridFragment->GetGridSize() * TileSize - 2 * GridFragment->GetGridPadding();
	FSlateBrush Brush;
	Brush.SetResourceObject(IconFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = IconSize * UWidgetLayoutLibrary::GetViewportScale(this);
	HoverItem->SetImageBrush(Brush);
	HoverItem->GridDimensions = GridFragment->GetGridSize();
	HoverItem->bIsStackable = Item->IsStackable();
	HoverItem->PreviousGridIndex = GridIndex;
	HoverItem->Item = Item;
	check(GridSlots.IsValidIndex(GridIndex))
	HoverItem->UpdateStackCount(GridSlots[GridIndex]->StackCount);
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, HoverItem);
}

void UInv_InventoryGrid::RemoveItemFromGrid(const UInv_InventoryItem* Item, int32 GridIndex)
{
	const FInv_GridFragment* GridFragment = Item->GetItemManifest().GetFragmentOfType<FInv_GridFragment>();
	UInv_InventoryStatics::ForEach2D(GridSlots, GridIndex, Column, GridFragment->GetGridSize(), [](UInv_GridSlot* GridSlot)
	{
		GridSlot->Item = nullptr;
		GridSlot->StackCount = 0;
		GridSlot->bAvailable = true;
		GridSlot->UpperLeftIndex = INDEX_NONE;
		GridSlot->SetUnoccupiedTexture();
	});
	if (SlottedItemWidgets.Contains(GridIndex))
	{
		TObjectPtr<UInv_SlottedItemWidget> SlottedItem;
		SlottedItemWidgets.RemoveAndCopyValue(GridIndex, SlottedItem);
		SlottedItem->RemoveFromParent();
	}
}

void UInv_InventoryGrid::ClearHoverItem()
{
	//移除随鼠标移动的临时物品图标
	HoverItem->RemoveFromParent();
	HoverItem = nullptr;
	//显示鼠标
	ShowCursor();
}

void UInv_InventoryGrid::OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (!IsValid(HoverItem))return;
	if (!GridSlots.IsValidIndex(ItemDropIndex))return;
	if (CurrQueryResult.Item.IsValid() && GridSlots.IsValidIndex(CurrQueryResult.UpperLeftIndex))
	{
		OnSlottedItemClicked(CurrQueryResult.UpperLeftIndex, MouseEvent);
		return;
	}
	if (CurrQueryResult.bHasSpace)
	{
		//放置物品
		CreateAndAddItemWidget(HoverItem->bIsStackable, HoverItem->Item.Get(), ItemDropIndex, HoverItem->GetStackCount());
		UpdateGridSlot(HoverItem->Item.Get(), ItemDropIndex, HoverItem->bIsStackable, HoverItem->GetStackCount());
		ClearHoverItem();
	}
}

void UInv_InventoryGrid::OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem))return;
	if (GridSlots[GridIndex]->bAvailable)
	{
		GridSlots[GridIndex]->SetOccupiedTexture();
	}
}

void UInv_InventoryGrid::OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem))return;
	if (GridSlots[GridIndex]->bAvailable)
	{
		GridSlots[GridIndex]->SetUnoccupiedTexture();
	}
}

UUserWidget* UInv_InventoryGrid::GetVisibleCursorWidget()
{
	if (!IsValid(GetOwningPlayer()))return nullptr;
	if (!IsValid(VisibleCursorWidget))
	{
		VisibleCursorWidget = CreateWidget(GetOwningPlayer(), VisibleCursorWidgetClass);
	}
	return VisibleCursorWidget;
}

UUserWidget* UInv_InventoryGrid::GetHiddenCursorWidget()
{
	if (!IsValid(GetOwningPlayer()))return nullptr;
	if (!IsValid(HiddenCursorWidget))
	{
		HiddenCursorWidget = CreateWidget(GetOwningPlayer(), HiddenCursorWidgetClass);
	}
	return HiddenCursorWidget;
}

void UInv_InventoryGrid::SplitItem(int32 SplitAmount, int32 GridIndex)
{
	UInv_InventoryItem* Item = GridSlots[GridIndex]->Item.Get();
	if (!IsValid(Item))return;
	if (!Item->IsStackable())return;
	GridSlots[GridIndex]->StackCount -= SplitAmount;
	SlottedItemWidgets[GridIndex]->UpdateStackCount(GridSlots[GridIndex]->StackCount);
	CreateHoverItem(Item, GridIndex);
	HoverItem->UpdateStackCount(SplitAmount);
}

void UInv_InventoryGrid::ConsumeItem(int32 GridIndex)
{
}

void UInv_InventoryGrid::DropItem(int32 GridIndex)
{
	UInv_InventoryItem* Item = GridSlots[GridIndex]->Item.Get();
	if (!IsValid(Item))return;
	InventoryComponent->Server_DropItem(Item, GridSlots[GridIndex]->StackCount);
	RemoveItemFromGrid(Item, GridIndex);
}

void UInv_InventoryGrid::DropHoverItem()
{
	if (!IsValid(HoverItem))return;
	if (!HoverItem->Item.IsValid())return;
	InventoryComponent->Server_DropItem(HoverItem->Item.Get(), HoverItem->GetStackCount());
	ClearHoverItem();
}

void UInv_InventoryGrid::ShowCursor()
{
	if (!IsValid(GetOwningPlayer()))return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetVisibleCursorWidget());
}

void UInv_InventoryGrid::HideCursor()
{
	if (!IsValid(GetOwningPlayer()))return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetHiddenCursorWidget());
}

void UInv_InventoryGrid::SetOwningCanvasPanel(UCanvasPanel* InCanvasPanel)
{
	OwningCanvasPanel = InCanvasPanel;
}
