#pragma once

#include "CSDefines.h"

#include "CSBaseObject.h"

#include "NIIteratedList.h"
#include "TES3DialogueFlags.h"

namespace se::cs {
	struct Dialogue : BaseObject {
		const char* id; // 0x10
		TES3::DialogueType type; // 0x14
		NI::IteratedList<DialogueInfo*> infos; // 0x18

		bool search(std::string_view needle, const SearchSettings& settings, std::regex* regex) const;
	};
	static_assert(sizeof(Dialogue) == 0x2C, "Dialogue failed size validation");
}
