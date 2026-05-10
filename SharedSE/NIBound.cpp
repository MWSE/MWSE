#include "NIBound.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

namespace NI {
	void Bound::computeFromData(unsigned int vertexCount, const Point3* vertices, unsigned int stride) {
#if defined(SE_NI_BOUND_FNADDR_COMPUTEFROMDATA) && SE_NI_BOUND_FNADDR_COMPUTEFROMDATA > 0
		const auto NI_Bound_computeFromData = reinterpret_cast<void(__thiscall*)(Bound*, unsigned int, const Point3*, unsigned int)>(SE_NI_BOUND_FNADDR_COMPUTEFROMDATA);
		NI_Bound_computeFromData(this, vertexCount, vertices, stride);
#else
		throw not_implemented_exception();
#endif
	}

	BoundingVolumeType BoundingVolume::getType() const {
		return vtbl->getType(this);
	}

	bool SphereBound::contains(const Point3& point) const {
		return center.distance(&point) <= radius;
	}

	Point3 SphereBound::getClosetPointTo(const Point3& point) const {
		if (contains(point)) {
			return point;
		}
		return center.interpolate(point, radius);
	}

	Point3 SphereBound::getFurthestPointFrom(const Point3& point) const {
		const auto distance = center.distance(&point);
		if (distance == 0.0f) {
			return { center.x, center.y + radius, center.z };
		}
		return center.interpolate(point, -radius);
	}

	BoxBoundingVolume* BoxBoundingVolume::create(const Point3& extent, const Point3& center, const Point3& xAxis, const Point3& yAxis, const Point3& zAxis) {
#if defined(SE_NI_BOXBOUNDINGVOLUME_FNADDR_CREATE) && SE_NI_BOXBOUNDINGVOLUME_FNADDR_CREATE > 0 && defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		const auto NI_BoxBoundingVolume_create = reinterpret_cast<BoxBoundingVolume * (__thiscall*)(BoxBoundingVolume*, const Point3&, const Point3&, const Point3&, const Point3&, const Point3&)>(SE_NI_BOXBOUNDINGVOLUME_FNADDR_CREATE);
		auto created = se::memory::_new<BoxBoundingVolume>();
		return NI_BoxBoundingVolume_create(created, extent, center, xAxis, yAxis, zAxis);
#else
		throw not_implemented_exception();
#endif
	}
}
