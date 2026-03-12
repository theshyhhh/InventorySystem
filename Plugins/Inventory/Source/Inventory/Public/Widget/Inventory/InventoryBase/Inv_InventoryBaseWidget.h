#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Type/Inv_GridType.h"
#include "Inv_InventoryBaseWidget.generated.h"

class UInv_ItemComponent;

UCLASS()
class INVENTORY_API UInv_InventoryBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComp) { return FInv_SlotAvailabilityResult(); }
};
