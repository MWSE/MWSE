#include "TES3SoundGenerator.h"

namespace TES3 {
	const char* SoundGenerator::getObjectID() const {
		return name;
	}

	std::string SoundGenerator::toJson() const {
		std::ostringstream ss;
		ss << "\"tes3soundGenerator:" << name << "\"";
		return std::move(ss.str());
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::SoundGenerator)
