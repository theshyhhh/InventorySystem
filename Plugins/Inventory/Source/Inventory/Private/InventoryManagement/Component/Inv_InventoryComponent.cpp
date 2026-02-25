#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "Widget/Inventory/InventoryBase/Inv_InventoryBaseWidget.h"


UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningPlayerController = Cast<APlayerController>(GetOwner());
	checkf(OwningPlayerController.IsValid(), TEXT("UInv_InventoryComponent应以PlayerController作为Owner"))
	if (!OwningPlayerController->IsLocalController())return;
	InventoryMenu = CreateWidget<UInv_InventoryBaseWidget>(OwningPlayerController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
}
