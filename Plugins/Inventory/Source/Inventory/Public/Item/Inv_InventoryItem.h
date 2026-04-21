#pragma once

#include "CoreMinimal.h"
#include "Item/Manifest/Inv_ItemManifest.h"
#include "UObject/Object.h"
#include "Inv_InventoryItem.generated.h"

/**
 * 用于代表物品本身的轻量级的类
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject
{
	GENERATED_BODY()

public:
	//成员变量需要被复制时，必须重写该函数
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * 检查该物品是否支持网络同步。
	 * @return 总是返回true，表示该物品支持网络同步。
	 */
	virtual bool IsSupportedForNetworking() const override { return true; }

	void SetItemManifest(const FInv_ItemManifest& InItemManifest);

	FORCEINLINE const FInv_ItemManifest& GetItemManifest() const { return ItemManifest.Get<FInv_ItemManifest>(); }

	FORCEINLINE FInv_ItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FInv_ItemManifest>(); }

	bool IsStackable() const;

	bool IsConsumable() const;

	//所有可堆叠该类物品的数目总和，不可堆叠固定为1
	UPROPERTY(Replicated)
	int32 TotalStackCount{0};

private:
	//限制可选地结构体类型为FInv_ItemManifest和它的派生结构体
	UPROPERTY(VisibleAnywhere, meta=(BaseStruct="/Script/Inventory.Inv_ItemManifest"), Replicated)
	FInstancedStruct ItemManifest;
};
