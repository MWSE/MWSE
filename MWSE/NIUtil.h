#pragma once

#include "NIDefines.h"
#include "TES3Defines.h"

namespace NI {
	Pick* getGlobalPick();

	TES3::Reference* getAssociatedReference(AVObject*);
}
