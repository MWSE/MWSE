#include "TES3Quest.h"

namespace TES3 {
	char* Quest::getObjectID() const {
		return name;
	}

	std::string Quest::toJson() const {
		std::ostringstream ss;
		ss << "\"tes3quest:" << getObjectID() << "\"";
		return std::move(ss.str());
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Quest)
