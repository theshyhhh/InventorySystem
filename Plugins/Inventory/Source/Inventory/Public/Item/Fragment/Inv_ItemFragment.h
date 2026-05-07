#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Inv_ItemFragment.generated.h"

class UInv_CompositeBase;

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

	virtual void Instantiated()
	{
	}

private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(Categories="FragmentTag"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FInv_InventoryFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UInv_CompositeBase* Composite) const;
};

USTRUCT(BlueprintType)
struct FInv_TextFragment : public FInv_InventoryFragment
{
	GENERATED_BODY()

	FText GetText() const { return Text; }

	virtual void Assimilate(UInv_CompositeBase* Composite) const override;

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FText Text;
};

USTRUCT(BlueprintType)
struct FInv_LabeledNumberFragment : public FInv_InventoryFragment
{
	GENERATED_BODY()

	virtual void Instantiated() override;

	virtual void Assimilate(UInv_CompositeBase* Composite) const override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Inventory")
	float Value{0};

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FText LabelText;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float MinValue{0};

	UPROPERTY(EditAnywhere, Category="Inventory")
	float MaxValue{0};


	/**
	 * 是否要进行随机取值，只在第一次实例化时进行随机取值，
	 */
	bool bRandomized{true};

	UPROPERTY(EditAnywhere, Category="Inventory")
	bool bCollapseLabel{false};

	UPROPERTY(EditAnywhere, Category="Inventory")
	bool bCollapseValue{false};
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
struct FInv_IconFragment : public FInv_InventoryFragment
{
	GENERATED_BODY()

	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }

	virtual void Assimilate(UInv_CompositeBase* Composite) const override;

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FVector2D IconSize{44.f, 44.f};
};


USTRUCT(BlueprintType)
struct FInv_StackableFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	FORCEINLINE int32 GetMaxStackSize() const { return MaxStackSize; }

	FORCEINLINE int32 GetStackCount() const { return StackCount; }

	void SetStackCount(int32 InStackCount)
	{
		StackCount = InStackCount;
	}

private:
	/**最大可堆叠数量*/
	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 MaxStackSize{1};

	/**当前数量*/
	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 StackCount{1};
};

USTRUCT(BlueprintType)
struct FInv_ConsumableModifier : public FInv_LabeledNumberFragment
{
	GENERATED_BODY()

	virtual void Consume(APlayerController* PC) const
	{
	}
};

USTRUCT(BlueprintType)
struct FInv_ConsumableFragment : public FInv_InventoryFragment
{
	GENERATED_BODY()

	virtual void Consume(APlayerController* PC);

	virtual void Assimilate(UInv_CompositeBase* Composite) const override;

	virtual void Instantiated() override;

private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ConsumableModifier>> ConsumableModifiers;
};

USTRUCT(BlueprintType)
struct FInv_RecoverHealthFragment : public FInv_ConsumableModifier
{
	GENERATED_BODY()

	virtual void Consume(APlayerController* PC) const override;
};

USTRUCT(BlueprintType)
struct FInv_RecoverManaFragment : public FInv_ConsumableModifier
{
	GENERATED_BODY()

	virtual void Consume(APlayerController* PC) const override;
};
