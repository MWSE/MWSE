#pragma once

namespace NI {
	struct Point2 {
		float x;
		float y;

		Point2();
		Point2(float x, float y);
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		Point2(sol::table table);
#endif

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		Point2& operator=(const sol::table table);
#endif

		bool operator==(const Point2& vector) const;
		bool operator!=(const Point2& vector) const;
		Point2 operator+(const Point2&) const;
		Point2 operator-(const Point2&) const;
		Point2 operator*(const Point2&) const;
		Point2 operator*(const float) const;
		Point2 operator/(const float) const;

		friend std::ostream& operator<<(std::ostream& str, const Point2& vector);
		std::string toString() const;
		std::string toJson() const;

		Point2 copy() const;
		Point2 min(const Point2& other) const;
		Point2 max(const Point2& other) const;

		float length() const;
		bool normalize();
		float distance(const Point2*) const;
		float distanceChebyshev(const Point2*) const;
		float distanceManhattan(const Point2*) const;

		Point2 normalized() const;

		const static Point2 UNIT_X;
		const static Point2 UNIT_NEG_X;
		const static Point2 UNIT_Y;
		const static Point2 UNIT_NEG_Y;
		const static Point2 ONES;
		const static Point2 ZEROES;
	};
}
