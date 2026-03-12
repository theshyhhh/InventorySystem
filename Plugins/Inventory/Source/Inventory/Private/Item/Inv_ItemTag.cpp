#include "Item/Inv_ItemTag.h"

namespace GameItem
{
	namespace Equipment
	{
		namespace Weapon
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Axe, "GameItem.Equipment.Weapon.Axe", "斧子")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Sword, "GameItem.Equipment.Weapon.Sword", "剑")
		}

		namespace Cloak
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(RedClock, "GameItem.Equipment.Cloak.RedClock", "红色斗篷")
		}

		namespace Mask
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(SteelMask, "GameItem.Equipment.Mask.SteelMask", "钢铁面具")
		}
	}

	namespace Consumable
	{
		namespace Potion
		{
			namespace Red
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Small, "GameItem.Consumable.Potion.Red.Small", "小红药")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Large, "GameItem.Consumable.Potion.Red.Large", "大红药")
			}

			namespace Blue
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Small, "GameItem.Consumable.Potion.Blue.Small", "小蓝药")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Large, "GameItem.Consumable.Potion.Blue.Large", "大蓝药")
			}
		}
	}

	namespace Craftable
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FireFernFruit, "GameItem.Craftable.FireFernFruit", "赤蕨浆果")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(LuminDaisy, "GameItem.Craftable.LuminDaisy", "光雏菊")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ScorchPetalBlossom, "GameItem.Craftable.ScorchPetalBlossom", "灼瓣花")
	}
}
