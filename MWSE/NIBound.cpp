#include "NIBound.h"

#include "MemoryUtil.h"

namespace NI {
	BoundingVolumeType BoundingVolume::getType() const {
		return vtbl->getType(this);
	}

	//
	// SphereBound
	//

	bool SphereBound::contains(const TES3::Vector3& point) const {
		return center.distance(&point) <= radius;
	}

	TES3::Vector3 SphereBound::getClosetPointTo(const TES3::Vector3& point) const {
		if (contains(point)) {
			return point;
		}
		return center.interpolate(point, radius);
	}

	TES3::Vector3 SphereBound::getFurthestPointFrom(const TES3::Vector3& point) const {
		const auto distance = center.distance(&point);
		if (distance == 0.0f) {
			return { center.x, center.y + radius, center.z };
		}
		return center.interpolate(point, -radius);
	}

	//
	// BoxBoundingVolume
	//

	const auto NI_BoxBoundingVolume_create = reinterpret_cast<BoxBoundingVolume * (__thiscall*)(BoxBoundingVolume*, const TES3::Vector3&, const TES3::Vector3&, const TES3::Vector3&, const TES3::Vector3&, const TES3::Vector3&)>(0x694C10);
	BoxBoundingVolume* BoxBoundingVolume::create(const TES3::Vector3& extent, const TES3::Vector3& center, const TES3::Vector3& xAxis, const TES3::Vector3& yAxis, const TES3::Vector3& zAxis) {
		auto created = mwse::tes3::_new<BoxBoundingVolume>();
		return NI_BoxBoundingVolume_create(created, extent, center, xAxis, yAxis, zAxis);
	}

}
