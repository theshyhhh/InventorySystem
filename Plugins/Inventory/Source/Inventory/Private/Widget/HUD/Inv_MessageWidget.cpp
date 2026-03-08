#include "Widget/HUD/Inv_MessageWidget.h"

#include "Components/TextBlock.h"

void UInv_MessageWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	HideMessage();
}

void UInv_MessageWidget::SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);
	if (!bIsMessageActive)
	{
		ShowMessage();
	}
	bIsMessageActive = true;
	GetWorld()->GetTimerManager().SetTimer(MessageTimerHandle, [this]()
	{
		HideMessage();
		bIsMessageActive = false;
	}, MessageLifeTime, false);
}
