#pragma intrinsic(_BitScanReverse)

#include "MathUtil.h"
#include "NIPoint3.h"

namespace se::math {
	void standardizeAngleRadians(float& value) {
		while (value > M_2PIf) {
			value -= M_2PIf;
		}
		while (value < 0.0f) {
			value += M_2PIf;
		}
	}

	std::tuple<float, NI::Point3> rayPlaneIntersection(
		const NI::Point3& rayOrigin, 
		const NI::Point3& rayDirection, 
		const NI::Point3& planeOrigin, 
		const NI::Point3& planeNormal
	) {
		// Note: This implementation intentionally allows intersections on either side of the plane.
		auto projectionDist = rayDirection.dotProduct(&planeNormal);
		if (projectionDist != 0.0f) {
			auto rayToPlaneDist = (planeOrigin - rayOrigin).dotProduct(&planeNormal);
			auto intersectionDist = rayToPlaneDist / projectionDist;
			if (intersectionDist >= 0.0f) {
				return { intersectionDist, rayOrigin + rayDirection * intersectionDist };
			}
		}
		return { -1.0, {} };
	}

	unsigned int roundDownToPowerOfTwo(unsigned int x) {
		if (x == 0) {
			return 0;
		}
		else {
			unsigned long int index;
			_BitScanReverse(&index, x);
			return (1u << index);
		}
	}
}
