#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Interaction/Inv_HighlightInterface.h"
#include "Inv_HighlightableStaticMeshComponent.generated.h"

UCLASS()
class INVENTORY_API UInv_HighlightableStaticMeshComponent : public UStaticMeshComponent, public IInv_HighlightInterface
{
	GENERATED_BODY()

public:
	//IInv_HighlightInterface Begin

	virtual void Highlight_Implementation() override;

	virtual void Unhighlight_Implementation() override;

	//IInv_HighlightInterface End

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};
