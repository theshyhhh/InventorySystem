#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Type/Inv_GridType.h"
#include "Inv_InventoryGrid.generated.h"

class UCanvasPanel;
class UInv_GridSlot;

UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	virtual void NativeOnInitialized() override;

private:
	void ConstructGrid();

	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	//格子的边长
	UPROPERTY(EditAnywhere, Category="Inventory")
	float TileSize;

	//行数
	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 Row;

	//列数
	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 Column;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess=true))
	EInv_ItemCategory ItemCategory = EInv_ItemCategory::None;
};
