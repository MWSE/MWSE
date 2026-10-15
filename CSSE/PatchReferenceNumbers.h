#pragma once

#include "CSDefines.h"

namespace se::cs::patch::reference_numbers {
	void onBeforeFilesLoaded();
	void onFilesLoaded(RecordHandler& recordHandler);
	void installPatches();
}
