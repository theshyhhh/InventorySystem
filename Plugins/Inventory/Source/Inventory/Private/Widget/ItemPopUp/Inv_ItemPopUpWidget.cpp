#include "Widget/ItemPopUp/Inv_ItemPopUpWidget.h"

#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UInv_ItemPopUpWidget::CollapseSplitButton() const
{
	Button_Split->SetVisibility(ESlateVisibility::Collapsed);
	Slider_Amount->SetVisibility(ESlateVisibility::Collapsed);
	TextBlock_Amount->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUpWidget::CollapseConsumeButton() const
{
	Button_Consume->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUpWidget::SetSliderMaxValue(float MaxValue) const
{
	Slider_Amount->SetMaxValue(MaxValue);
	TextBlock_Amount->SetText(FText::AsNumber(Slider_Amount->GetValue()));
}

FVector2D UInv_ItemPopUpWidget::GetRootSizeBoxSize() const
{
	return FVector2D(SizeBox_Root->GetWidthOverride(), SizeBox_Root->GetHeightOverride());
}

void UInv_ItemPopUpWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button_Split->OnClicked.AddDynamic(this, &UInv_ItemPopUpWidget::OnSplitButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &UInv_ItemPopUpWidget::OnConsumeButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &UInv_ItemPopUpWidget::OnDropButtonClicked);
	Slider_Amount->OnValueChanged.AddDynamic(this, &UInv_ItemPopUpWidget::OnAmountSliderValueChanged);
}

void UInv_ItemPopUpWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}

void UInv_ItemPopUpWidget::OnSplitButtonClicked()
{
	OnSplitButtonClickedDelegate.ExecuteIfBound(FMath::Floor(Slider_Amount->GetValue()), GridIndex);
	RemoveFromParent();
}

void UInv_ItemPopUpWidget::OnConsumeButtonClicked()
{
	OnConsumeButtonClickedDelegate.ExecuteIfBound(GridIndex);
	RemoveFromParent();
}

void UInv_ItemPopUpWidget::OnDropButtonClicked()
{
	OnDropButtonClickedDelegate.ExecuteIfBound(GridIndex);
	RemoveFromParent();
}

void UInv_ItemPopUpWidget::OnAmountSliderValueChanged(float Value)
{
	TextBlock_Amount->SetText(FText::AsNumber(FMath::Floor(Value)));
}
