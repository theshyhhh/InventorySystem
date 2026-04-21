#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemPopUpWidget.generated.h"

class USizeBox;
class UTextBlock;
class USlider;
class UButton;

DECLARE_DELEGATE_TwoParams(FOnSplitButtonClickedSignature, int32/**物品数量*/, int32/**索引*/);

DECLARE_DELEGATE_OneParam(FOnDropButtonClickedSignature, int32);

DECLARE_DELEGATE_OneParam(FOnConsumeButtonClickedSignature, int32);

/**
 * 右键点击道具时，弹出的小部件
 */
UCLASS()
class INVENTORY_API UInv_ItemPopUpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void CollapseSplitButton() const;

	void CollapseConsumeButton() const;

	void SetSliderMaxValue(float MaxValue) const;

	FVector2D GetRootSizeBoxSize() const;

	FOnSplitButtonClickedSignature OnSplitButtonClickedDelegate;

	FOnDropButtonClickedSignature OnDropButtonClickedDelegate;

	FOnConsumeButtonClickedSignature OnConsumeButtonClickedDelegate;

	//记录哪个格子打开的菜单
	int32 GridIndex;

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UFUNCTION()
	void OnSplitButtonClicked();

	UFUNCTION()
	void OnConsumeButtonClicked();

	UFUNCTION()
	void OnDropButtonClicked();

	UFUNCTION()
	void OnAmountSliderValueChanged(float Value);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Split;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Consume;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Drop;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USlider> Slider_Amount;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Amount;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
};
