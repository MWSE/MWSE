#pragma once

#include "NITriShapeData.h"

namespace NI {
	struct TriShapeDynamicData : TriShapeData {
		unsigned short activeVertices; // 0x48
		unsigned short activeTriangles; // 0x4A
	};
	static_assert(sizeof(TriShapeDynamicData) == 0x4C, "NI::TriShapeDynamicData failed size validation");
}
