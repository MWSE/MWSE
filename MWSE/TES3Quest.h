#pragma once

#include "TES3Object.h"

#include "TES3IteratedList.h"

namespace TES3 {
	struct Quest : BaseObject {
		char* name; // 0x10
		IteratedList<Dialogue*> dialogues; // 0x14
		IteratedList<DialogueInfo*> activeInfos; // 0x28

		static constexpr auto OBJECT_TYPE = ObjectType::Quest;

		Quest() = delete;
		~Quest() = delete;

		//
		// Virtual table overrides.
		//

		char* getObjectID() const;

		//
		// Custom functions.
		//

		std::string toJson() const;

		bool isActive() const;
		bool isFinished() const;
		bool isStarted() const;

	};
	static_assert(sizeof(Quest) == 0x3C, "TES3::Quest failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Quest)
