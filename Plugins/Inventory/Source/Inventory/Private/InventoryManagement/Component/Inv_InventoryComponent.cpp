#include "InventoryManagement/Component/Inv_InventoryComponent.h"

#include "Item/Inv_InventoryItem.h"
#include "Item/Component/Inv_ItemComponent.h"
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
		Server_AddStacksToItem(ItemComp, Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0)
	{
		//如果库存中没有该物品，或该物品不可堆叠，则需要创建这个物品，并更新格子显示的内容
		Server_AddNewItem(ItemComp, Result.bStackable ? Result.TotalRoomToFill : 0);
	}
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
	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		//因为AddEntry时调用的PostReplicatedAdd只在复制到客户端时调用，所以这里要保证在监听服务器和单机下委托的广播
		OnItemAddedDelegate.Broadcast(Item);
	}
	//TODO: 摧毁地上的原物品
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComp, int32 StackCount, int32 Remainder)
{
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
