#pragma once

#include "NiVector3.h"

namespace se::math {
	constexpr auto M_PI = 3.14159265358979323846; // pi
	constexpr auto M_PIf = float(M_PI);

	constexpr auto M_2PI = 2.0 * M_PI; // 2pi
	constexpr auto M_2PIf = float(M_2PI);

	constexpr double M_PI_2 = M_PI / 2; // pi/2
	constexpr double M_PI_2f = float(M_PI_2);

	inline float radiansToDegrees(float radians) {
		return float(radians * 180.0 / M_PI);
	}

	inline float degreesToRadians(float degrees) {
		return float(degrees * M_PI / 180.0);
	}

	void standardizeAngleRadians(float& value);
	
	std::tuple<float, NI::Vector3> rayPlaneIntersection(
		const NI::Vector3& rayOrigin, 
		const NI::Vector3& rayDirection, 
		const NI::Vector3& planeOrigin, 
		const NI::Vector3& planeNormal
	);

	unsigned int roundDownToPowerOfTwo(unsigned int x);
}
