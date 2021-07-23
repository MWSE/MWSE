#include "TES3Item.h"

#include "TES3ItemData.h"

namespace TES3 {
	ItemData * Item::createItemData() {
		return ItemData::createForObject(this);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Item)
