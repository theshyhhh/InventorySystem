#include "Widget/Composite/Inv_Leaf_LabeledValue.h"

#include "Components/TextBlock.h"

void UInv_Leaf_LabeledValue::SetLabelText(const FText& LabelText, bool bCollapse) const
{
	if (bCollapse)
	{
		TextBlock_Label->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	TextBlock_Label->SetText(LabelText);
}

void UInv_Leaf_LabeledValue::SetValueText(const FText& ValueText, bool bCollapse) const
{
	if (bCollapse)
	{
		TextBlock_Value->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	TextBlock_Value->SetText(ValueText);
}

void UInv_Leaf_LabeledValue::NativePreConstruct()
{
	Super::NativePreConstruct();
	FSlateFontInfo LabelFontInfo = TextBlock_Label->GetFont();
	LabelFontInfo.Size = FontSize_Label;
	TextBlock_Label->SetFont(LabelFontInfo);

	FSlateFontInfo ValueFontInfo = TextBlock_Value->GetFont();
	ValueFontInfo.Size = FontSize_Value;
	TextBlock_Value->SetFont(ValueFontInfo);
}
