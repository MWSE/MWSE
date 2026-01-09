#pragma once

#include "NIGeometry.h"
#include "NiTriBasedGeometryData.h"

namespace NI {
	struct TriBasedGeometry_vTable : Geometry_vTable {

	};
	static_assert(sizeof(TriBasedGeometry_vTable) == 0x9C, "NI::TriBasedGeometry_vTable failed size validation");

	struct TriBasedGeometry : Geometry {

		TriBasedGeometry(TriBasedGeometryData* data);

		//
		// vTable type overwriting.
		//

		bool findIntersections(const TES3::Vector3* position, const TES3::Vector3* direction, Pick* pick);
		Pointer<TriBasedGeometryData> getModelData() const;

	};
	static_assert(sizeof(TriBasedGeometry) == 0xAC, "NI::TriBasedGeometry failed size validation");
}
