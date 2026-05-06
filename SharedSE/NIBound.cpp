#include "NIBound.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

namespace NI {
	void Bound::computeFromData(unsigned int vertexCount, const Vector3* vertices, unsigned int stride) {
#if defined(SE_NI_BOUND_FNADDR_COMPUTEFROMDATA) && SE_NI_BOUND_FNADDR_COMPUTEFROMDATA > 0
		const auto NI_Bound_computeFromData = reinterpret_cast<void(__thiscall*)(Bound*, unsigned int, const Vector3*, unsigned int)>(SE_NI_BOUND_FNADDR_COMPUTEFROMDATA);
		NI_Bound_computeFromData(this, vertexCount, vertices, stride);
#else
		throw not_implemented_exception();
#endif
	}

	BoundingVolumeType BoundingVolume::getType() const {
		return vtbl->getType(this);
	}

	bool SphereBound::contains(const Vector3& point) const {
		return center.distance(&point) <= radius;
	}

	Vector3 SphereBound::getClosetPointTo(const Vector3& point) const {
		if (contains(point)) {
			return point;
		}
		return center.interpolate(point, radius);
	}

	Vector3 SphereBound::getFurthestPointFrom(const Vector3& point) const {
		const auto distance = center.distance(&point);
		if (distance == 0.0f) {
			return { center.x, center.y + radius, center.z };
		}
		return center.interpolate(point, -radius);
	}

	BoxBoundingVolume* BoxBoundingVolume::create(const Vector3& extent, const Vector3& center, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) {
#if defined(SE_NI_BOXBOUNDINGVOLUME_FNADDR_CREATE) && SE_NI_BOXBOUNDINGVOLUME_FNADDR_CREATE > 0 && defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		const auto NI_BoxBoundingVolume_create = reinterpret_cast<BoxBoundingVolume * (__thiscall*)(BoxBoundingVolume*, const Vector3&, const Vector3&, const Vector3&, const Vector3&, const Vector3&)>(SE_NI_BOXBOUNDINGVOLUME_FNADDR_CREATE);
		auto created = se::memory::_new<BoxBoundingVolume>();
		return NI_BoxBoundingVolume_create(created, extent, center, xAxis, yAxis, zAxis);
#else
		throw not_implemented_exception();
#endif
	}
}
