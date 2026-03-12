#pragma once

#include "NativeGameplayTags.h"

namespace GameItem
{
	namespace Equipment
	{
		namespace Weapon
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Axe)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sword)
		}

		namespace Cloak
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RedClock)
		}

		namespace Mask
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(SteelMask)
		}
	}

	namespace Consumable
	{
		namespace Potion
		{
			namespace Red
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small)
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large)
			}

			namespace Blue
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Small)
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(Large)
			}
		}
	}

	namespace Craftable
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireFernFruit)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(LuminDaisy)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ScorchPetalBlossom)
	}
}
