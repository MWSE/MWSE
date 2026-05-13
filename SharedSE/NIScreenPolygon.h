#pragma once

#include "NIObject.h"
#include "NIPoint2.h"
#include "NIPoint3.h"

namespace NI {
	struct ScreenPolygon : Object {
		void* propertyState; // 0x8
		unsigned short vertexCount; // 0xC
		Point3* vertices; // 0x10
		Point2* texture; // 0x14
		PackedColor* color; // 0x18
	};
	static_assert(sizeof(ScreenPolygon) == 0x1C, "NI::ScreenPolygon failed size validation");
}
