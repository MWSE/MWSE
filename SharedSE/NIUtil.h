#pragma once

#include "NIDefines.h"

namespace TES3 {
	struct Reference;
}

namespace NI {
	Pick* getGlobalPick();

	TES3::Reference* getAssociatedReference(AVObject*);
}
