#pragma once

#include "Inv_GridType.generated.h"

UENUM(BlueprintType)
enum class EInv_ItemCategory:uint8
{
	Equippable,
	Consumable,
	Craftable,
	None
};
