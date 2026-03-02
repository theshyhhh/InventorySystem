#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;

UCLASS()
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	int32 Index;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_GridSlot;
};
