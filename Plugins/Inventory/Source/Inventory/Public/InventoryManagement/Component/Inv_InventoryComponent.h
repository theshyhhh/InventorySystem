#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_InventoryComponent.generated.h"


class UInv_ItemComponent;
class UInv_InventoryItem;
class UInv_InventoryBaseWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChangeSignature, UInv_InventoryItem*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryNoRoomSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	//切换库存菜单的显隐
	void ToggleInventoryMenu();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComp);

	FOnInventoryItemChangeSignature OnItemAddedDelegate;

	FOnInventoryItemChangeSignature OnItemRemovedDelegate;

	FInventoryNoRoomSignature OnInventoryNoRoomDelegate;

protected:
	virtual void BeginPlay() override;

private:
	void ConstructInventory();

	/**打开库存菜单 显示鼠标*/
	void OpenInventoryMenu();

	/**关闭库存菜单 隐藏鼠标*/
	void CloseInventoryMenu();

	TWeakObjectPtr<APlayerController> OwningPlayerController;

	UPROPERTY()
	TObjectPtr<UInv_InventoryBaseWidget> InventoryMenu;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UInv_InventoryBaseWidget> InventoryMenuClass;

	bool bInventoryMenuOpen = false;
};
