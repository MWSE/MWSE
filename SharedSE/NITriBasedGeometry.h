#pragma once

#include "NIGeometry.h"
#include "NITriBasedGeometryData.h"

namespace NI {
	struct TriBasedGeometry_vTable : Geometry_vTable {

	};
	static_assert(sizeof(TriBasedGeometry_vTable) == 0x9C, "NI::TriBasedGeometry_vTable failed size validation");

	struct TriBasedGeometry : Geometry {

		TriBasedGeometry(TriBasedGeometryData* data);

		//
		// vTable type overwriting.
		//

		bool findIntersections(const Vector3* position, const Vector3* direction, Pick* pick);
		Pointer<TriBasedGeometryData> getModelData() const;

		// Inline non-const overload retained for CSSE/inline call sites.
		Pointer<TriBasedGeometryData> getModelData() { return static_cast<TriBasedGeometryData*>(modelData.get()); }

	};
	static_assert(sizeof(TriBasedGeometry) == 0xAC, "NI::TriBasedGeometry failed size validation");
}
