#include "Widget/Inventory/HoverItem/Inv_HoverItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/Inv_InventoryItem.h"

void UInv_HoverItem::SetImageBrush(const FSlateBrush& Brush)
{
	Image_Icon->SetBrush(Brush);
}

void UInv_HoverItem::UpdateStackCount(int32 Count)
{
	StackCount = Count;
	if (bIsStackable)
	{
		TextBlock_StackCount->SetText(FText::AsNumber(Count));
		TextBlock_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TextBlock_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FGameplayTag UInv_HoverItem::GetItemTag() const
{
	if (Item.IsValid())
	{
		return Item->GetItemManifest().GetItemTag();
	}
	return FGameplayTag::EmptyTag;
}
