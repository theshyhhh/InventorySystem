#pragma once

#include "CoreMinimal.h"
#include "Inv_Leaf.h"
#include "Inv_Leaf_LabeledValue.generated.h"

class UTextBlock;

UCLASS()
class INVENTORY_API UInv_Leaf_LabeledValue : public UInv_Leaf
{
	GENERATED_BODY()

public:
	void SetLabelText(const FText& LabelText, bool bCollapse) const;

	void SetValueText(const FText& ValueText, bool bCollapse) const;

protected:
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Label;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Value;

	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 FontSize_Label{15};

	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 FontSize_Value{15};
};
