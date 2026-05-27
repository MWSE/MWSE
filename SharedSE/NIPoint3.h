#pragma once

#include "NIDefines.h"

namespace NI {
	struct Point3 {
		float x; // 0x0
		float y; // 0x4
		float z; // 0x8

		friend struct Matrix33;

		Point3();
		Point3(float x, float y, float z);
		Point3(const NI::Color& color);
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		Point3(const sol::table& table);
		Point3(const sol::object& object);
#endif

		Point3& operator=(const NI::Color& vector);
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		Point3& operator=(const sol::table& table);
		Point3& operator=(const sol::object& object);
#endif

		bool operator==(const Point3& vector) const;
		bool operator!=(const Point3& vector) const;
		Point3 operator+(const Point3&) const;
		Point3 operator+(const float) const;
		Point3 operator-() const;
		Point3 operator-(const Point3&) const;
		Point3 operator-(const float) const;
		Point3 operator*(const Point3&) const;
		Point3 operator*(const float) const;
		Point3 operator/(const float) const;

		friend std::ostream& operator<<(std::ostream& str, const Point3& vector);
		std::string toString() const;
		std::string toJson() const;

		//
		// Associated functions.
		//

		Point3 copy() const;
		NI::Color toNiColor() const;

		Point3 min(const Point3& other) const;
		Point3 max(const Point3& other) const;

		Point3 crossProduct(const Point3*) const;
		float dotProduct(const Point3*) const;
		Matrix33 outerProduct(const Point3*) const;

		Point3 lerp(const Point3& to, float transition) const;
		float heightDifference(const Point3*) const;
		float distance(const Point3*) const;
		float distanceChebyshev(const Point3*) const;
		float distanceManhattan(const Point3*) const;
		float distanceXY(const Point3*) const;
		float angle(const Point3*) const;
		float length() const;
		void negate();
		bool normalize();
		Point3 normalized() const;
		Point3 interpolate(const Point3&, const float) const;

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		static bool canConvertFrom(const sol::table& table);
#endif

		const static Point3 UNIT_X;
		const static Point3 UNIT_NEG_X;
		const static Point3 UNIT_Y;
		const static Point3 UNIT_NEG_Y;
		const static Point3 UNIT_Z;
		const static Point3 UNIT_NEG_Z;
		const static Point3 ONES;
		const static Point3 ZEROES;
		const static Point3 MIN;
		const static Point3 MAX;

	};
	static_assert(sizeof(Point3) == 0xC, "NI::Point3 failed size validation");
}
