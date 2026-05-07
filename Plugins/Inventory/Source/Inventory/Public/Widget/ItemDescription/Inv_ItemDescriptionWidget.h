#pragma once

#include "CoreMinimal.h"
#include "Widget/Composite/Inv_Composite.h"
#include "Inv_ItemDescriptionWidget.generated.h"

class USizeBox;

UCLASS()
class INVENTORY_API UInv_ItemDescriptionWidget : public UInv_Composite
{
	GENERATED_BODY()

public:
	FVector2D GetSizeBox() const;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
