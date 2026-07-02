#pragma once

#include "NICollisionGroup.h"
#include "NIGeometry.h"
#include "NITriBasedGeometryData.h"

namespace NI {
	struct TriBasedGeometry_vTable : Geometry_vTable {
		void* unknown_0x9C;
		void* unknown_0xA0;
	};
	static_assert(sizeof(TriBasedGeometry_vTable) == 0xA4, "NI::TriBasedGeometry_vTable failed size validation");

	struct TriBasedGeometry : Geometry {

		TriBasedGeometry(TriBasedGeometryData* data);

		//
		// vTable type overwriting.
		//

		bool findIntersections(const Point3* position, const Point3* direction, Pick* pick);
		int findCollisionsTriVsABV(float fTime, AVObject* collidee, char bCalcNormals, CollisionGroup::Intersect* intersect);
		Pointer<TriBasedGeometryData> getModelData() const;

		Pointer<TriBasedGeometryData> getModelData() { return static_cast<TriBasedGeometryData*>(modelData.get()); }

	};
	static_assert(sizeof(TriBasedGeometry) == 0xAC, "NI::TriBasedGeometry failed size validation");
}
