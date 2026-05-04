#pragma once

#include "NIDefines.h"
#include "NIVector3.h"

namespace NI {
	struct Bound {
		Vector3 center; // 0x0
		float radius; // 0xC

		void computeFromData(unsigned int vertexCount, const Vector3* vertices, unsigned int stride);
	};
	static_assert(sizeof(Bound) == 0x10, "NI::Bound failed size validation");
}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1

// MWSE polymorphic bounding-volume hierarchy. Used by occlusion culling,
// picking, and other scene-graph queries. Not used by CSSE -- the entire
// hierarchy is hidden in non-MWSE builds.
//
// SphereBound is structurally identical to Bound (Vector3 center + float
// radius, 0x10 bytes) but is a separate C++ type per MWSE-original. Methods
// on SphereBound (contains, getClosetPointTo, getFurthestPointFrom) are
// MWSE-specific and live in MWSE-private NIBound.cpp.

#include "NIMatrix33.h"

namespace NI {
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
		Vector3 center; // 0x0
		float radius; // 0xC

		bool contains(const Vector3& point) const;
		Vector3 getClosetPointTo(const Vector3& point) const;
		Vector3 getFurthestPointFrom(const Vector3& point) const;
	};
	static_assert(sizeof(SphereBound) == 0x10, "NI::Bound failed size validation");

	struct SphereBoundingVolume : TypedBoundingVolume<SphereBound> {
	};

	struct BoxBound {
		Vector3 center; // 0x4
		Matrix33 basis; // 0xC
		Vector3 extent; // 0x30
	};
	static_assert(sizeof(BoxBound) == 0x3C, "NI::SphereBV failed size validation");

	struct BoxBoundingVolume : TypedBoundingVolume<BoxBound> {
		static BoxBoundingVolume* create(const Vector3& extent, const Vector3& center, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
	};
	static_assert(sizeof(BoxBoundingVolume) == 0x40, "NI::SphereBV failed size validation");
}

#endif
