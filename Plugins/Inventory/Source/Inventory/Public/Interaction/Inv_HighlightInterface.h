#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inv_HighlightInterface.generated.h"

UINTERFACE()
class UInv_HighlightInterface : public UInterface
{
	GENERATED_BODY()
};

class INVENTORY_API IInv_HighlightInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="Inventory")
	void Highlight();

	UFUNCTION(BlueprintNativeEvent, Category="Inventory")
	void Unhighlight();
};
