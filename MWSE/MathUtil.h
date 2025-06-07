#pragma once

namespace mwse::math {
	constexpr auto M_E = 2.71828182845904523536; // e
	constexpr auto M_LOG2E = 1.44269504088896340736; // log2(e)
	constexpr auto M_LOG10E = 0.434294481903251827651; // log10(e)
	constexpr auto M_LN2 = 0.693147180559945309417; // ln(2)
	constexpr auto M_LN10 = 2.30258509299404568402; // ln(10)
	constexpr auto M_PI = 3.14159265358979323846; // pi
	constexpr auto M_PI_2 = 1.57079632679489661923; // pi/2
	constexpr auto M_PI_4 = 0.785398163397448309616; // pi/4
	constexpr auto M_1_PI = 0.318309886183790671538; // 1/pi
	constexpr auto M_2_PI = 0.636619772367581343076; // 2/pi
	constexpr auto M_2_SQRTPI = 1.12837916709551257390; // 2/sqrt(pi)
	constexpr auto M_SQRT2 = 1.41421356237309504880; // sqrt(2)
	constexpr auto M_SQRT1_2 = 0.707106781186547524401; // 1/sqrt(2)
	constexpr auto M_NORMALIZE_EPSILON = 1e-6f; // Epsilon for NiVector normalization.
	
	inline float lerp(float a, float b, float t) {
		return a + t * (b - a);
	}

	inline double lerp(double a, double b, double t) {
		return a + t * (b - a);
	}
	
	template <typename T>
	inline bool near(T a, T b, T e = std::numeric_limits<T>::epsilon()) {
		return std::abs(a - b) < e;
	}
}
