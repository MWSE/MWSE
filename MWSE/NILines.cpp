#include "NiLines.h"

#include "NiLinesData.h"

namespace NI {
	LinesData* Lines::getModelData() const {
		return static_cast<LinesData*>(modelData.get());
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Lines)
