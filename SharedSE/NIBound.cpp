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

	bool Bound::contains(const Point3& point) const {
		return center.distance(&point) <= radius;
	}

	Point3 Bound::getClosetPointTo(const Point3& point) const {
		if (contains(point)) {
			return point;
		}
		return center.interpolate(point, radius);
	}

	Point3 Bound::getFurthestPointFrom(const Point3& point) const {
		const auto distance = center.distance(&point);
		if (distance == 0.0f) {
			return { center.x, center.y + radius, center.z };
		}
		return center.interpolate(point, -radius);
	}

	BoundingVolumeType BoundingVolume::getType() const {
		return vtbl->getType(this);
	}

	std::optional<BoundingBox> BoundingVolume::computeBoundingBox() const {
		switch (getType()) {
		case BoundingVolumeType::Sphere:
		{
			const auto& sphere = static_cast<const SphereBoundingVolume*>(this)->bounds;
			const Point3 extent = { sphere.radius, sphere.radius, sphere.radius };
			return BoundingBox(sphere.center - extent, sphere.center + extent);
		}
		case BoundingVolumeType::Box:
		{
			const auto& box = static_cast<const BoxBoundingVolume*>(this)->bounds;
			// The basis holds the box axes. Take the larger of the row-vector and
			// column-vector interpretations per world axis so the result bounds
			// the box regardless of storage convention.
			const auto& m = box.basis;
			const auto& e = box.extent;
			const Point3 rows = {
				std::fabs(m.m0.x) * e.x + std::fabs(m.m1.x) * e.y + std::fabs(m.m2.x) * e.z,
				std::fabs(m.m0.y) * e.x + std::fabs(m.m1.y) * e.y + std::fabs(m.m2.y) * e.z,
				std::fabs(m.m0.z) * e.x + std::fabs(m.m1.z) * e.y + std::fabs(m.m2.z) * e.z,
			};
			const Point3 cols = {
				std::fabs(m.m0.x) * e.x + std::fabs(m.m0.y) * e.y + std::fabs(m.m0.z) * e.z,
				std::fabs(m.m1.x) * e.x + std::fabs(m.m1.y) * e.y + std::fabs(m.m1.z) * e.z,
				std::fabs(m.m2.x) * e.x + std::fabs(m.m2.y) * e.y + std::fabs(m.m2.z) * e.z,
			};
			const Point3 extent = { std::max(rows.x, cols.x), std::max(rows.y, cols.y), std::max(rows.z, cols.z) };
			return BoundingBox(box.center - extent, box.center + extent);
		}
		case BoundingVolumeType::Union:
		{
			// An unhandled member makes the whole union unhandled.
			const auto& children = static_cast<const UnionBoundingVolume*>(this)->children;
			std::optional<BoundingBox> result;
			for (const auto child : children) {
				if (!child) {
					continue;
				}
				const auto childBox = child->computeBoundingBox();
				if (!childBox) {
					return {};
				}
				if (!result) {
					result = childBox;
				}
				else {
					result->minimum = { std::min(result->minimum.x, childBox->minimum.x), std::min(result->minimum.y, childBox->minimum.y), std::min(result->minimum.z, childBox->minimum.z) };
					result->maximum = { std::max(result->maximum.x, childBox->maximum.x), std::max(result->maximum.y, childBox->maximum.y), std::max(result->maximum.z, childBox->maximum.z) };
				}
			}
			return result;
		}
		default:
			return {};
		}
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
