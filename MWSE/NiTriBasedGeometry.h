#pragma once

#include "NIGeometry.h"
#include "NiTriBasedGeometryData.h"

namespace NI {
	struct TriBasedGeometry_vTable : Geometry_vTable {

	};
	static_assert(sizeof(TriBasedGeometry_vTable) == 0x9C, "NI::TriBasedGeometry_vTable failed size validation");

	struct TriBasedGeometry : Geometry {

		//
		// vTable type overwriting.
		//

		TriBasedGeometryData* getModelData() { return static_cast<TriBasedGeometryData*>(modelData.get()); }

	};
	static_assert(sizeof(TriBasedGeometry) == 0xAC, "NI::TriBasedGeometry failed size validation");
}
