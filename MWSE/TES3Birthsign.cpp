#include "TES3Birthsign.h"

namespace TES3 {
	char* Birthsign::getName() {
		return name;
	}

	const auto TES3_Birthsign_getDescription = reinterpret_cast<char* (__thiscall*)(Birthsign*)>(0x474990);
	char* Birthsign::getDescription() {
		return TES3_Birthsign_getDescription(this);
	}

	const auto TES3_Birthsign_freeDescription = reinterpret_cast<void (__thiscall*)(Birthsign*)>(0x474B70);
	void Birthsign::freeDescription() {
		return TES3_Birthsign_freeDescription(this);
	}

	std::string Birthsign::getAndFreeDescription() {
		std::string desc = getDescription();
		freeDescription();
		return std::move(desc);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Birthsign)
