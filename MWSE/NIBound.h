#pragma once

#include "TES3Vectors.h"

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
		TES3::Vector3 center; // 0x0
		float radius; // 0xC

		bool contains(const TES3::Vector3& point) const;
		TES3::Vector3 getClosetPointTo(const TES3::Vector3& point) const;
		TES3::Vector3 getFurthestPointFrom(const TES3::Vector3& point) const;
	};
	static_assert(sizeof(SphereBound) == 0x10, "NI::Bound failed size validation");
	struct SphereBoundingVolume : TypedBoundingVolume<SphereBound> {
	};

	struct BoxBound {
		TES3::Vector3 center; // 0x4
		TES3::Matrix33 basis; // 0xC
		TES3::Vector3 extent; // 0x30
	};
	static_assert(sizeof(BoxBound) == 0x3C, "NI::SphereBV failed size validation");

	struct BoxBoundingVolume : TypedBoundingVolume<BoxBound> {
		static BoxBoundingVolume* create(const TES3::Vector3& extent, const TES3::Vector3& center, const TES3::Vector3& xAxis, const TES3::Vector3& yAxis, const TES3::Vector3& zAxis);
	};
	static_assert(sizeof(BoxBoundingVolume) == 0x40, "NI::SphereBV failed size validation");
}
