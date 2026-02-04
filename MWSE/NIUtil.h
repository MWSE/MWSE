#pragma once

#include "NIDefines.h"
#include "TES3Defines.h"

namespace NI {
	Pick* getGlobalPick();

	TES3::Reference* getAssociatedReference(AVObject*);

	bool passesTraverseFilters(const AVObject* object, const std::unordered_set<unsigned int>& typeFilters, std::string_view prefix);
}
