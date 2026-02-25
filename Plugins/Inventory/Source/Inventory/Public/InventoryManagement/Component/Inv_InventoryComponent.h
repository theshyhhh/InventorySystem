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

protected:
	virtual void BeginPlay() override;

private:
	void ConstructInventory();

	TWeakObjectPtr<APlayerController> OwningPlayerController;

	UPROPERTY()
	TObjectPtr<UInv_InventoryBaseWidget> InventoryMenu;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UInv_InventoryBaseWidget> InventoryMenuClass;
};
