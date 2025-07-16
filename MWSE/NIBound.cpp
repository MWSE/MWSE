#include "NIBound.h"

namespace NI {
	bool Bound::contains(const TES3::Vector3& point) const {
		return center.distance(&point) <= radius;
	}

	TES3::Vector3 Bound::getClosetPointTo(const TES3::Vector3& point) const {
		if (contains(point)) {
			return point;
		}
		return center.interpolate(point, radius);
	}

	TES3::Vector3 Bound::getFurthestPointFrom(const TES3::Vector3& point) const {
		const auto distance = center.distance(&point);
		if (distance == 0.0f) {
			return { center.x, center.y + radius, center.z };
		}
		return center.interpolate(point, -radius);
	}
}
