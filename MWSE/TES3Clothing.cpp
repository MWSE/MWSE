#include "TES3Clothing.h"

#include "TES3Util.h"

namespace TES3 {
	void Clothing::setIconPath(const char* path) {
		if (strnlen_s(path, 32) >= 32) {
			throw std::invalid_argument("Path must not be 32 or more characters.");
		}
		mwse::tes3::setDataString(&icon, path);
	}

	const auto TES3_Clothing_getSlotName = reinterpret_cast<const char* (__thiscall*)(Clothing*)>(0x4A38E0);
	const char* Clothing::getSlotName() {
		// If this armor has weight and is of an invalid slot, return straight up armor rating.
		if (slot < ClothingSlot::First || slot > ClothingSlot::Last) {
			auto slotData = mwse::tes3::getClothingSlotData(slot);
			if (slotData) {
				return slotData->name.c_str();
			}

			return "Unknown";
		}

		return TES3_Clothing_getSlotName(this);
	}

	std::reference_wrapper<WearablePart[7]> Clothing::getParts() {
		return std::ref(parts);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Clothing)
