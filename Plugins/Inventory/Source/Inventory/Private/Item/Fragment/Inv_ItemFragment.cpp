#include "Item/Fragment/Inv_ItemFragment.h"


#include "Widget/Composite/Inv_CompositeBase.h"
#include "Widget/Composite/Inv_Leaf_Image.h"
#include "Widget/Composite/Inv_Leaf_Text.h"
#include "Widget/Composite/Inv_Leaf_LabeledValue.h"

void FInv_InventoryFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag()))return;
	Composite->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void FInv_TextFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag()))return;
	Composite->SetVisibility(ESlateVisibility::HitTestInvisible);

	UInv_Leaf_Text* Leaf_Text = Cast<UInv_Leaf_Text>(Composite);
	if (!IsValid(Leaf_Text))return;
	Leaf_Text->SetText(Text);
}

void FInv_LabeledNumberFragment::Instantiated()
{
	FInv_InventoryFragment::Instantiated();
	if (bRandomized)
	{
		Value = FMath::RandRange(MinValue, MaxValue);
		bRandomized = false;
	}
}

void FInv_LabeledNumberFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag()))return;
	Composite->SetVisibility(ESlateVisibility::HitTestInvisible);
	UInv_Leaf_LabeledValue* LabeledValue = Cast<UInv_Leaf_LabeledValue>(Composite);
	LabeledValue->SetLabelText(LabelText, bCollapseLabel);
	FNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 1;
	LabeledValue->SetValueText(FText::AsNumber(Value, &Options), bCollapseValue);
}

void FInv_IconFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag()))return;
	Composite->SetVisibility(ESlateVisibility::HitTestInvisible);
	UInv_Leaf_Image* Image = Cast<UInv_Leaf_Image>(Composite);
	if (!IsValid(Image))return;
	Image->SetImage(Icon);
	Image->SetImageSize(IconSize);
}

void FInv_ConsumableFragment::Consume(APlayerController* PC)
{
	for (const TInstancedStruct<FInv_ConsumableModifier>& Modifier : ConsumableModifiers)
	{
		Modifier.Get().Consume(PC);
	}
}

void FInv_ConsumableFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	for (const TInstancedStruct<FInv_ConsumableModifier>& Modifier : ConsumableModifiers)
	{
		Modifier.Get().Assimilate(Composite);
	}
}

void FInv_ConsumableFragment::Instantiated()
{
	for (TInstancedStruct<FInv_ConsumableModifier>& Modifier : ConsumableModifiers)
	{
		Modifier.GetMutable().Instantiated();
	}
}

void FInv_RecoverHealthFragment::Consume(APlayerController* PC) const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s恢复了%f点血量"), *PC->GetName(), Value));
}

void FInv_RecoverManaFragment::Consume(APlayerController* PC) const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s恢复了%f点蓝量"), *PC->GetName(), Value));
}
