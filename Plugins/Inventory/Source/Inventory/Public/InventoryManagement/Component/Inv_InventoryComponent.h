#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_InventoryComponent.generated.h"


class UInv_InventoryBaseWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	//切换库存菜单的显隐
	void ToggleInventoryMenu();

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
