#pragma once

#include <numbers>
#include "NIPoint3.h"

namespace se::math {
	constexpr auto M_PI_2 = 1.57079632679489661923; // pi/2
	constexpr auto M_PI_2f = float(M_PI_2);
	constexpr auto M_PI_4 = 0.785398163397448309616; // pi/4
	constexpr auto M_2PI = 2.0 * std::numbers::pi; // 2pi
	constexpr auto M_2PIf = float(M_2PI);
	constexpr auto M_2_PI = 0.636619772367581343076; // 2/pi
	constexpr auto M_2_SQRTPI = 1.12837916709551257390; // 2/sqrt(pi)
	constexpr auto M_SQRT1_2 = 0.707106781186547524401; // 1/sqrt(2)
	constexpr auto M_NORMALIZE_EPSILON = 1e-6f; // Epsilon for NiVector normalization.

	inline float radiansToDegrees(float radians) {
		return radians * 180.0f / std::numbers::pi_v<float>;
	}

	inline float degreesToRadians(float degrees) {
		return degrees * std::numbers::pi_v<float> / 180.0f;
	}

	// Recalculates rotation to always be between [0, 2pi].
	void standardizeAngleRadians(float& value);
	
	std::tuple<float, NI::Point3> rayPlaneIntersection(
		const NI::Point3& rayOrigin, 
		const NI::Point3& rayDirection, 
		const NI::Point3& planeOrigin, 
		const NI::Point3& planeNormal
	);

	unsigned int roundDownToPowerOfTwo(unsigned int x);
}
