#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_ItemFragment.generated.h"

USTRUCT(BlueprintType)
struct FInv_ItemFragment
{
	GENERATED_BODY()

	FInv_ItemFragment()
	{
	}

	FInv_ItemFragment(const FInv_ItemFragment&) = default;
	FInv_ItemFragment& operator=(const FInv_ItemFragment&) = default;
	FInv_ItemFragment(FInv_ItemFragment&&) = default;
	FInv_ItemFragment& operator=(FInv_ItemFragment&&) = default;

	virtual ~FInv_ItemFragment()
	{
	}

	FORCEINLINE FGameplayTag GetFragmentTag() const { return FragmentTag; }

private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(Categories="FragmentTag"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	FORCEINLINE FIntPoint GetGridSize() const { return GridSize; }

	FORCEINLINE float GetGridPadding() const { return GridPadding; }

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FIntPoint GridSize{1, 1};

	UPROPERTY(EditAnywhere, Category="Inventory")
	float GridPadding{0.f};
};

USTRUCT(BlueprintType)
struct FInv_IconFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FVector2D IconSize{44.f, 44.f};
};
