#pragma once

#include "NIDefines.h"

namespace NI {
	struct Vector3 {
		float x; // 0x0
		float y; // 0x4
		float z; // 0x8

		friend struct Matrix33;

		Vector3();
		Vector3(float x, float y, float z);
		Vector3(NI::Color& color);
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		Vector3(sol::table table);
		Vector3(sol::object object);
#endif

		Vector3& operator=(const NI::Color& vector);
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		Vector3& operator=(const sol::table table);
		Vector3& operator=(const sol::object object);
#endif

		bool operator==(const Vector3& vector) const;
		bool operator!=(const Vector3& vector) const;
		Vector3 operator+(const Vector3&) const;
		Vector3 operator+(const float) const;
		Vector3 operator-() const;
		Vector3 operator-(const Vector3&) const;
		Vector3 operator-(const float) const;
		Vector3 operator*(const Vector3&) const;
		Vector3 operator*(const float) const;
		Vector3 operator/(const float) const;

		friend std::ostream& operator<<(std::ostream& str, const Vector3& vector);
		std::string toString() const;
		std::string toJson() const;

		//
		// Associated functions.
		//

		Vector3 copy() const;
		NI::Color toNiColor() const;

		Vector3 min(const Vector3& other) const;
		Vector3 max(const Vector3& other) const;

		Vector3 crossProduct(const Vector3*) const;
		float dotProduct(const Vector3*) const;
		Matrix33 outerProduct(const Vector3*) const;

		Vector3 lerp(const Vector3& to, float transition) const;
		float heightDifference(const Vector3*) const;
		float distance(const Vector3*) const;
		float distanceChebyshev(const Vector3*) const;
		float distanceManhattan(const Vector3*) const;
		float distanceXY(const Vector3*) const;
		float angle(const Vector3*) const;
		float length() const;
		void negate();
		bool normalize();
		Vector3 normalized() const;
		Vector3 interpolate(const Vector3&, const float) const;

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		static bool canConvertFrom(sol::table& table);
#endif

		const static Vector3 UNIT_X;
		const static Vector3 UNIT_NEG_X;
		const static Vector3 UNIT_Y;
		const static Vector3 UNIT_NEG_Y;
		const static Vector3 UNIT_Z;
		const static Vector3 UNIT_NEG_Z;
		const static Vector3 ONES;
		const static Vector3 ZEROES;
		const static Vector3 MIN;
		const static Vector3 MAX;

	};
	static_assert(sizeof(Vector3) == 0xC, "NI::Vector3 failed size validation");
}
