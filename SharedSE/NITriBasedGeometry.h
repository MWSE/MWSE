#pragma once

#include "NIGeometry.h"
#include "NiTriBasedGeometryData.h"

namespace NI {
	struct TriBasedGeometry_vTable : Geometry_vTable {
		void* unknown_0x9C;
		void* unknown_0xA0;
	};
	static_assert(sizeof(TriBasedGeometry_vTable) == 0xA4, "NI::TriBasedGeometry's vtable failed size validation");

	struct TriBasedGeometry : Geometry {

		//
		// vTable type overwriting.
		//

		Pointer<TriBasedGeometryData> getModelData() { return static_cast<TriBasedGeometryData*>(modelData.get()); }

	};
	static_assert(sizeof(TriBasedGeometry) == 0xAC, "NI::TriBasedGeometry failed size validation");
}
