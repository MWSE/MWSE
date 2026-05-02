#include "TES3Quest.h"

#include "TES3DialogueInfo.h"

namespace TES3 {
	char* Quest::getObjectID() const {
		return name;
	}

	std::string Quest::toJson() const {
		std::ostringstream ss;
		ss << "\"tes3quest:" << getObjectID() << "\"";
		return std::move(ss.str());
	}

	bool Quest::isActive() const {
		return isStarted() && !isFinished();
	}

	bool Quest::isStarted() const {
		return (objectFlags & TES3::ObjectFlag::QuestStarted) != 0;
	}

	bool Quest::isFinished() const {
		return (objectFlags & TES3::ObjectFlag::QuestFinished) != 0;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Quest)
