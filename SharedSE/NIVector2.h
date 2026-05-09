#pragma once

namespace NI {
	struct Vector2 {
		float x;
		float y;

		Vector2();
		Vector2(float x, float y);
		Vector2(sol::table table);

		Vector2& operator=(const sol::table table);

		bool operator==(const Vector2& vector) const;
		bool operator!=(const Vector2& vector) const;
		Vector2 operator+(const Vector2&) const;
		Vector2 operator-(const Vector2&) const;
		Vector2 operator*(const Vector2&) const;
		Vector2 operator*(const float) const;
		Vector2 operator/(const float) const;

		friend std::ostream& operator<<(std::ostream& str, const Vector2& vector);
		std::string toString() const;
		std::string toJson() const;

		Vector2 copy() const;
		Vector2 min(const Vector2& other) const;
		Vector2 max(const Vector2& other) const;

		float length() const;
		bool normalize();
		float distance(const Vector2*) const;
		float distanceChebyshev(const Vector2*) const;
		float distanceManhattan(const Vector2*) const;

		Vector2 normalized() const;

		const static Vector2 UNIT_X;
		const static Vector2 UNIT_NEG_X;
		const static Vector2 UNIT_Y;
		const static Vector2 UNIT_NEG_Y;
		const static Vector2 ONES;
		const static Vector2 ZEROES;
	};
}
