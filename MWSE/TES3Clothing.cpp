#include "TES3Clothing.h"

#include "TES3Util.h"

#include "TES3BodyPartManager.h"

namespace TES3 {
	const auto TES3_Clothing_ctor = reinterpret_cast<void(__thiscall*)(Clothing*)>(0x4A2C70);
	Clothing::Clothing() {
		TES3_Clothing_ctor(this);
	}

	const auto TES3_Clothing_dtor = reinterpret_cast<void(__thiscall*)(Clothing*)>(0x4A2D60);
	Clothing::~Clothing() {
		TES3_Clothing_dtor(this);
	}

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

	bool Clothing::isUsableByBeasts() const {
		constexpr auto leftFootID = static_cast<int>(TES3::BodyPartManager::ActiveBodyPart::Index::LeftFoot);
		constexpr auto rightFootID = static_cast<int>(TES3::BodyPartManager::ActiveBodyPart::Index::RightFoot);
		for (auto& part : parts) {
			if (part.bodypartID == leftFootID || part.bodypartID == rightFootID) {
				return false;
			}
		}
		return true;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Clothing)
