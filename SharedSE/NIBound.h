#pragma once

#include "NIDefines.h"
#include "NIMatrix33.h"
#include "NIPoint3.h"

namespace NI {
	struct Bound {
		Point3 center; // 0x0
		float radius; // 0xC

		void computeFromData(unsigned int vertexCount, const Point3* vertices, unsigned int stride);

		// A recreation of NiBound::Merge, but in our own code so the compiler can optimize.
		// Force inline because compiler being dumb, and that's the whole point of all this.
		void __forceinline merge(const Bound& other) {
			float dx = center.x - other.center.x;
			float dy = center.y - other.center.y;
			float dz = center.z - other.center.z;
			float distSq = dx * dx + dy * dy + dz * dz;

			float radiusDiff = other.radius - radius;
			float radiusDiffSq = radiusDiff * radiusDiff;

			// NaN check to match vanilla behavior
			if (distSq != distSq || radiusDiff != radiusDiff) {
				return;
			}

			if (radiusDiffSq >= distSq) {
				if (radiusDiff >= 0.0f) {
					*this = other;
				}
				return;
			}

			// Confirmed in IDA that the compiler wasn't doing this optimization itself.
			// `sqrtf` lowered to an out-of-line call, and clobbered volatile registers.
			float dist = _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(distSq))); // sqrtf(distSq)
			if (dist > 1e-6f) {
				float factor = (dist - radiusDiff) / (2.0f * dist);
				center.x = other.center.x + dx * factor;
				center.y = other.center.y + dy * factor;
				center.z = other.center.z + dz * factor;
			}
			radius = (radius + other.radius + dist) * 0.5f;
		}

		bool contains(const Point3& point) const;
		Point3 getClosetPointTo(const Point3& point) const;
		Point3 getFurthestPointFrom(const Point3& point) const;
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

	struct Sphere {
		Point3 center; // 0x0
		float radius; // 0xC
	};

	struct SphereBoundingVolume : TypedBoundingVolume<Sphere> {
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
