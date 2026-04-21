#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Inv_InventoryComponent.generated.h"


struct FInv_SlotAvailabilityResult;
class UInv_ItemComponent;
class UInv_InventoryBaseWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChangeSignature, UInv_InventoryItem*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryNoRoomSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemStackChangeSignature, const FInv_SlotAvailabilityResult&, Result);

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

	UFUNCTION(Server, Reliable)
	void Server_DropItem(UInv_InventoryItem* Item, int32 StackCount);

	/**
	 * 添加要随该组件复制的UObject
	 * @param SubObj 要随该组件复制的SubObj
	 */
	void AddSubObject(UObject* SubObj);

	FOnInventoryItemChangeSignature OnItemAddedDelegate;

	FOnInventoryItemChangeSignature OnItemRemovedDelegate;

	FInventoryNoRoomSignature OnInventoryNoRoomDelegate;

	FOnItemStackChangeSignature OnItemStackChangeDelegate;

	virtual void BeginPlay() override;

private:
	void ConstructInventory();

	/**打开库存菜单 显示鼠标*/
	void OpenInventoryMenu();

	/**关闭库存菜单 隐藏鼠标*/
	void CloseInventoryMenu();

	void SpawnDroppedItem(const UInv_InventoryItem* Item, int32 Count) const;

	UPROPERTY(Replicated)
	FInv_InventoryFastArraySerializer InventoryList;

	TWeakObjectPtr<APlayerController> OwningPlayerController;

	UPROPERTY()
	TObjectPtr<UInv_InventoryBaseWidget> InventoryMenu;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UInv_InventoryBaseWidget> InventoryMenuClass;

	bool bInventoryMenuOpen = false;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	float SpawnItemAngle = 80.f;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	float MaxSpawnDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	float MinSpawnDistance = 30.f;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	float SpawnHeight = 70.f;
};
