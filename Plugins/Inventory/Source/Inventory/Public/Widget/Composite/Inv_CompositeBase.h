#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Inv_CompositeBase.generated.h"

UCLASS()
class INVENTORY_API UInv_CompositeBase : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE FGameplayTag GetFragmentTag() const
	{
		return FragmentTag;
	}

	using FuncType = TFunction<void(UInv_CompositeBase*)>;

	virtual void ApplyFunction(FuncType Func)
	{
	}

private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(Categories="FragmentTag"))
	FGameplayTag FragmentTag;
};
