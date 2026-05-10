#pragma once

#include "NIDefines.h"
#include "NIMatrix33.h"
#include "NIPoint3.h"

// Polymorphic bounding-volume hierarchy used by the NetImmerse engine for
// occlusion culling, picking, and scene-graph queries. The struct layouts
// describe the actual engine memory; method implementations live in
// MWSE-private NIBound.cpp (CSSE does not currently call them, so the
// declarations exist on the CSSE side as forward API surface only).
//
// SphereBound is structurally identical to Bound (Point3 center + float
// radius, 0x10 bytes) but is a separate C++ type per MWSE-original.

namespace NI {
	struct Bound {
		Point3 center; // 0x0
		float radius; // 0xC

		void computeFromData(unsigned int vertexCount, const Point3* vertices, unsigned int stride);
	};
	static_assert(sizeof(Bound) == 0x10, "NI::Bound failed size validation");

	enum class BoundingVolumeType : int {
		Sphere,
		Box,
		Capsule,
		Lozenge,
		Union,
		HalfSpace,
		ENUM_MAX,
	};

	struct BoundingVolume_vtbl {
		void* load;
		void* save;
		void* dtor;
		BoundingVolumeType(__thiscall* getType)(const BoundingVolume*);
		void* whichObjectIntersect;
		void* updateWorldData;
		void* create;
		void* clone;
		void* copy;
		void* operator_eq;
		void* operator_not_eq;
		void* addViewerStrings;
	};
	static_assert(sizeof(BoundingVolume_vtbl) == 0x30, "NI::BoundingVolume's vtable failed size validation");

	struct BoundingVolume {
		BoundingVolume_vtbl* vtbl; // 0x0

		BoundingVolumeType getType() const;
	};
	static_assert(sizeof(BoundingVolume) == 0x4, "NI::BoundingVolume failed size validation");

	template <typename T>
	struct TypedBoundingVolume : BoundingVolume {
		T bounds; // 0x4
	};

	struct SphereBound {
		Point3 center; // 0x0
		float radius; // 0xC

		bool contains(const Point3& point) const;
		Point3 getClosetPointTo(const Point3& point) const;
		Point3 getFurthestPointFrom(const Point3& point) const;
	};
	static_assert(sizeof(SphereBound) == 0x10, "NI::Bound failed size validation");

	struct SphereBoundingVolume : TypedBoundingVolume<SphereBound> {
	};

	struct BoxBound {
		Point3 center; // 0x4
		Matrix33 basis; // 0xC
		Point3 extent; // 0x30
	};
	static_assert(sizeof(BoxBound) == 0x3C, "NI::SphereBV failed size validation");

	struct BoxBoundingVolume : TypedBoundingVolume<BoxBound> {
		static BoxBoundingVolume* create(const Point3& extent, const Point3& center, const Point3& xAxis, const Point3& yAxis, const Point3& zAxis);
	};
	static_assert(sizeof(BoxBoundingVolume) == 0x40, "NI::SphereBV failed size validation");
}
