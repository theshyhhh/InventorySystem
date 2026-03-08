#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_MessageWidget.generated.h"

class UTextBlock;

UCLASS()
class INVENTORY_API UInv_MessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void ShowMessage();

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void HideMessage();

	void SetMessage(const FText& Message);

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Message;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float MessageLifeTime{4.f};

	FTimerHandle MessageTimerHandle;

	bool bIsMessageActive{false};
};
