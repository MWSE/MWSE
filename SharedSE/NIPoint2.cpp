#include "NIPoint2.h"

#include "MathUtil.h"

namespace NI {
	const Point2 Point2::UNIT_X = { 1.0f, 0.0f };
	const Point2 Point2::UNIT_NEG_X = { -1.0f, 0.0f };
	const Point2 Point2::UNIT_Y = { 0.0f, 1.0f };
	const Point2 Point2::UNIT_NEG_Y = { 0.0f, -1.0f };
	const Point2 Point2::ONES = { 1.0f, 1.0f };
	const Point2 Point2::ZEROES = { 0.0f, 0.0f };

	Point2::Point2() :
		x(0),
		y(0)
	{

	}

	Point2::Point2(float _x, float _y) :
		x(_x),
		y(_y)
	{

	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Point2::Point2(sol::table table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
	}
#endif

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Point2& Point2::operator=(const sol::table table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
		return *this;
	}
#endif

	bool Point2::operator==(const Point2& vec3) const {
		return x == vec3.x && y == vec3.y;
	}

	bool Point2::operator!=(const Point2& vec3) const {
		return x != vec3.x || y != vec3.y;
	}

	Point2 Point2::operator+(const Point2& vec3) const {
		return Point2(x + vec3.x, y + vec3.y);
	}

	Point2 Point2::operator-(const Point2& vec3) const {
		return Point2(x - vec3.x, y - vec3.y);
	}

	Point2 Point2::operator*(const Point2& vec3) const {
		return Point2(x * vec3.x, y * vec3.y);
	}

	Point2 Point2::operator*(const float scalar) const {
		return Point2(x * scalar, y * scalar);
	}

	Point2 Point2::operator/(const float scalar) const {
		return { x / scalar, y / scalar };
	}

	std::ostream& operator<<(std::ostream& str, const Point2& vector) {
		str << "(" << vector.x << "," << vector.y << ")";
		return str;
	}

	float Point2::distance(const Point2* vec2) const {
		float dx = x - vec2->x;
		float dy = y - vec2->y;
		return sqrt(dx * dx + dy * dy);
	}


	float Point2::distanceChebyshev(const Point2* vec2) const {
		float dx = fabs(x - vec2->x);
		float dy = fabs(y - vec2->y);
		return std::max(dx, dy);
	}

	float Point2::distanceManhattan(const Point2* vec2) const {
		float dx = fabs(x - vec2->x);
		float dy = fabs(y - vec2->y);
		return dx + dy;
	}

	std::string Point2::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Point2::toJson() const {
		std::ostringstream ss;
		ss << "{\"x\":" << x << ",\"y\":" << y << "}";
		return std::move(ss.str());
	}

	Point2 Point2::copy() const {
		return *this;
	}

	Point2 Point2::min(const Point2& other) const {
		return Point2(std::min(x, other.x), std::min(y, other.y));
	}

	Point2 Point2::max(const Point2& other) const {
		return Point2(std::max(x, other.x), std::max(y, other.y));
	}

	float Point2::length() const {
		return sqrt(x * x + y * y);
	}

	bool Point2::normalize() {
		float len = length();
		if (len > se::math::M_NORMALIZE_EPSILON) {
			x = x / len;
			y = y / len;
			return true;
		}
		x = 0;
		y = 0;
		return false;
	}

	Point2 Point2::normalized() const {
		auto copy = Point2(x, y);
		copy.normalize();
		return copy;
	}
}
