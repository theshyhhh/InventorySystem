#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Inv_InventoryComponent.generated.h"


class UInv_ItemComponent;
class UInv_InventoryBaseWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChangeSignature, UInv_InventoryItem*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryNoRoomSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//切换库存菜单的显隐
	void ToggleInventoryMenu();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComp);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UInv_ItemComponent* ItemComp, int32 StackCount);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UInv_ItemComponent* ItemComp, int32 StackCount, int32 Remainder);

	/**
	 * 添加要随该组件复制的UObject
	 * @param SubObj 要随该组件复制的SubObj
	 */
	void AddSubObject(UObject* SubObj);

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

	UPROPERTY(Replicated)
	FInv_InventoryFastArraySerializer InventoryList;

	TWeakObjectPtr<APlayerController> OwningPlayerController;

	UPROPERTY()
	TObjectPtr<UInv_InventoryBaseWidget> InventoryMenu;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UInv_InventoryBaseWidget> InventoryMenuClass;

	bool bInventoryMenuOpen = false;
};
