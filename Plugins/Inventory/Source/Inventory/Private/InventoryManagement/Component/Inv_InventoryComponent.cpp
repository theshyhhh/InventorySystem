#include "InventoryManagement/Component/Inv_InventoryComponent.h"

#include "Inventory.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Item/Inv_InventoryItem.h"
#include "Item/Component/Inv_ItemComponent.h"
#include "Item/Fragment/Inv_ItemFragment.h"
#include "Net/UnrealNetwork.h"
#include "Widget/Inventory/InventoryBase/Inv_InventoryBaseWidget.h"

UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	InventoryList.OwningComponent = this;
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, InventoryList);
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComp)
{
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComp);
	Result.Item = InventoryList.FindFirstItemByItemTag(ItemComp->GetItemManifest().GetItemTag());

	if (Result.TotalRoomToFill == 0)
	{
		OnInventoryNoRoomDelegate.Broadcast();
		return;
	}
	if (Result.Item.IsValid() && Result.bStackable)
	{
		//如果库存中有该物品，且该物品可堆叠，则只需改变堆叠数量
		OnItemStackChangeDelegate.Broadcast(Result);
		Server_AddStacksToItem(ItemComp, Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0)
	{
		//如果库存中没有该物品，或该物品不可堆叠，则需要创建这个物品，并更新格子显示的内容
		Server_AddNewItem(ItemComp, Result.bStackable ? Result.TotalRoomToFill : 0);
	}
}

void UInv_InventoryComponent::Server_DropItem_Implementation(UInv_InventoryItem* Item, int32 StackCount)
{
	const int32 NewStackCount = Item->TotalStackCount - StackCount;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->TotalStackCount = NewStackCount;
	}
	//生成丢弃后的物品
	SpawnDroppedItem(Item, StackCount);
}

void UInv_InventoryComponent::SpawnDroppedItem(const UInv_InventoryItem* Item, int32 Count) const
{
	const APawn* OwningPawn = OwningPlayerController->GetPawn();
	check(OwningPawn);
	FVector RotatedForward = OwningPawn->GetActorForwardVector().
	                                     RotateAngleAxis(FMath::RandRange(-SpawnItemAngle, SpawnItemAngle), FVector::UpVector);
	FVector SpawnLocation = RotatedForward * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance) + OwningPawn->GetActorLocation();
	SpawnLocation.Z += SpawnHeight;
	FInv_ItemManifest ItemManifest = Item->GetItemManifest();
	if (FInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(Count);
	}

	UInv_InventoryStatics::SpawnItemByItemManifest(this, ItemManifest, SpawnLocation, RotatedForward.Rotation());
}

void UInv_InventoryComponent::AddSubObject(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		//子对象必须重写IsSupportedForNetworking
		AddReplicatedSubObject(SubObj);
	}
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComp, int32 StackCount)
{
	UInv_InventoryItem* Item = InventoryList.AddEntry(ItemComp);
	Item->TotalStackCount = StackCount;
	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		//因为AddEntry时调用的PostReplicatedAdd只在复制到客户端时调用，所以这里要保证在监听服务器和单机下委托的广播
		OnItemAddedDelegate.Broadcast(Item);
	}
	ItemComp->PickedUp();
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComp, int32 StackCount, int32 Remainder)
{
	UInv_InventoryItem* Item = InventoryList.FindFirstItemByItemTag(ItemComp->GetItemManifest().GetItemTag());
	if (!IsValid(Item))
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryList未找到有效的Item"));
		return;
	}
	Item->TotalStackCount += StackCount;
	if (Remainder == 0)
	{
		ItemComp->PickedUp();
	}
	else if (FInv_StackableFragment* StackableFragment = ItemComp->GetItemManifest().GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu))return;
	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;
	if (!OwningPlayerController.IsValid())return;
	FInputModeGameAndUI InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(true);
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu))return;
	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;
	if (!OwningPlayerController.IsValid())return;
	FInputModeGameOnly InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(false);
}

void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningPlayerController = Cast<APlayerController>(GetOwner());
	checkf(OwningPlayerController.IsValid(), TEXT("UInv_InventoryComponent应以PlayerController作为Owner"))
	//只有本地玩家才创建UI
	if (!OwningPlayerController->IsLocalController())return;
	InventoryMenu = CreateWidget<UInv_InventoryBaseWidget>(OwningPlayerController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
}
