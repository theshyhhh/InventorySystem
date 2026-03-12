#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SlottedItemWidget.generated.h"

class UInv_InventoryItem;
class UImage;

UCLASS()
class INVENTORY_API UInv_SlottedItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UImage* GetImageIcon() const { return Image_Icon; }

	int32 GridIndex;

	FIntPoint GridSize;

	TWeakObjectPtr<UInv_InventoryItem> Item;

	bool bIsStackable;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;
};
