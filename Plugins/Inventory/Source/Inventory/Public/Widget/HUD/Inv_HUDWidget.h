#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HUDWidget.generated.h"

class UInv_MessageWidget;

UCLASS()
class INVENTORY_API UInv_HUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void ShowPickUpMessage(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void HidePickUpMessage();

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInv_MessageWidget> MessageWidget;

	UFUNCTION()
	void OnInventoryNoRoom();
};
