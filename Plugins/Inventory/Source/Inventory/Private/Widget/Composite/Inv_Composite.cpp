#include "Widget/Composite/Inv_Composite.h"

#include "Blueprint/WidgetTree.h"

void UInv_Composite::ApplyFunction(FuncType Func)
{
	Super::ApplyFunction(Func);

	for (UInv_CompositeBase* Child : Children)
	{
		Child->ApplyFunction(Func);
	}
}

void UInv_Composite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UInv_CompositeBase* CompositeBase = Cast<UInv_CompositeBase>(Widget))
		{
			Children.Add(CompositeBase);
			CompositeBase->SetVisibility(ESlateVisibility::Collapsed);
		}
	});
}
