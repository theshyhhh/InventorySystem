#include "Item/Fragment/Inv_FragmentTag.h"

namespace FragmentTag
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GridFragment, "FragmentTag.GridFragment", "格子布局")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IconFragment, "FragmentTag.IconFragment", "图标")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StackableFragment, "FragmentTag.StackableFragment", "可堆叠数")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ConsumableFragment, "FragmentTag.ConsumableFragment", "可消耗属性")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(NameFragment, "FragmentTag.NameFragment", "名字属性")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(PrimaryStatFragment, "FragmentTag.PrimaryStatFragment", "道具主属性")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TypeFragment, "FragmentTag.TypeFragment", "道具类型")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(FlavorTextFragment, "FragmentTag.FlavorTextFragment", "简介")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SellingPriceFragment, "FragmentTag.SellingPriceFragment", "售价")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(RequiredLevelFragment, "FragmentTag.RequiredLevelFragment", "使用需求等级")

	namespace StatMod
	{
		UE_DEFINE_GAMEPLAY_TAG(StatMod_1, "FragmentTag.StatMod.1")
		UE_DEFINE_GAMEPLAY_TAG(StatMod_2, "FragmentTag.StatMod.2")
		UE_DEFINE_GAMEPLAY_TAG(StatMod_3, "FragmentTag.StatMod.3")
	}
}
