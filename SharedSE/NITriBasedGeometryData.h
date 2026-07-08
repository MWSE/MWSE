#pragma once

#include "NIGeometryData.h"

namespace NI {
	struct TriBasedGeometryData_vTable : GeometryData_vTable {
		void(__thiscall* setActiveTriangleCount)(TriBasedGeometryData*, unsigned short); // 0x38
		unsigned short(__thiscall* getActiveTriangleCount)(const TriBasedGeometryData*); // 0x3C
		Triangle* (__thiscall* getTriList)(TriBasedGeometryData*); // 0x40
		const Triangle* (__thiscall* getTriList_const)(const TriBasedGeometryData*); // 0x44
		void(__thiscall* getTriangleIndices)(const TriBasedGeometryData*, unsigned short, unsigned short&, unsigned short&, unsigned short&); // 0x48
	};
	static_assert(sizeof(TriBasedGeometryData_vTable) == 0x4C, "NI::TriBasedGeometryData_vTable failed size validation");

	struct TriBasedGeometryData : GeometryData {
		unsigned short triangleCount; // 0x34
		unsigned short patchRenderFlags; // 0x36 // Reused padding space

		//
		// vTable wrappers.
		//

		Triangle* getTriList();
		const Triangle* getTriList() const;
		unsigned short getActiveTriangleCount() const;
		void setActiveTriangleCount(unsigned short count);

		//
		// Custom functions.
		//

		// Returns ascending indices of active triangles a model-space ray may hit,
		// or nullptr when the caller must test every triangle. The result is a
		// shared scratch buffer, valid until the next candidate query. Not thread-safe.
		const std::vector<unsigned int>* getRayCandidateTriangles(const Point3& modelOrigin, const Point3& modelDirection) const;

		// Same contract as getRayCandidateTriangles, for a model-space AABB tested
		// against all triangles rather than the active count.
		const std::vector<unsigned int>* getAabbCandidateTriangles(const Point3& modelAabbMin, const Point3& modelAabbMax) const;

	};
	static_assert(sizeof(TriBasedGeometryData) == 0x38, "NI::TriBasedGeometryData failed size validation");
}
