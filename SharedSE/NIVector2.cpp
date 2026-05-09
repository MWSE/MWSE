#include "NIVector2.h"

#include "MathUtil.h"

namespace NI {
	const Vector2 Vector2::UNIT_X = { 1.0f, 0.0f };
	const Vector2 Vector2::UNIT_NEG_X = { -1.0f, 0.0f };
	const Vector2 Vector2::UNIT_Y = { 0.0f, 1.0f };
	const Vector2 Vector2::UNIT_NEG_Y = { 0.0f, -1.0f };
	const Vector2 Vector2::ONES = { 1.0f, 1.0f };
	const Vector2 Vector2::ZEROES = { 0.0f, 0.0f };

	Vector2::Vector2() :
		x(0),
		y(0)
	{

	}

	Vector2::Vector2(float _x, float _y) :
		x(_x),
		y(_y)
	{

	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Vector2::Vector2(sol::table table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
	}
#endif

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Vector2& Vector2::operator=(const sol::table table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
		return *this;
	}
#endif

	bool Vector2::operator==(const Vector2& vec3) const {
		return x == vec3.x && y == vec3.y;
	}

	bool Vector2::operator!=(const Vector2& vec3) const {
		return x != vec3.x || y != vec3.y;
	}

	Vector2 Vector2::operator+(const Vector2& vec3) const {
		return Vector2(x + vec3.x, y + vec3.y);
	}

	Vector2 Vector2::operator-(const Vector2& vec3) const {
		return Vector2(x - vec3.x, y - vec3.y);
	}

	Vector2 Vector2::operator*(const Vector2& vec3) const {
		return Vector2(x * vec3.x, y * vec3.y);
	}

	Vector2 Vector2::operator*(const float scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 Vector2::operator/(const float scalar) const {
		return { x / scalar, y / scalar };
	}

	std::ostream& operator<<(std::ostream& str, const Vector2& vector) {
		str << "(" << vector.x << "," << vector.y << ")";
		return str;
	}

	float Vector2::distance(const Vector2* vec2) const {
		float dx = x - vec2->x;
		float dy = y - vec2->y;
		return sqrt(dx * dx + dy * dy);
	}


	float Vector2::distanceChebyshev(const Vector2* vec2) const {
		float dx = fabs(x - vec2->x);
		float dy = fabs(y - vec2->y);
		return std::max(dx, dy);
	}

	float Vector2::distanceManhattan(const Vector2* vec2) const {
		float dx = fabs(x - vec2->x);
		float dy = fabs(y - vec2->y);
		return dx + dy;
	}

	std::string Vector2::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Vector2::toJson() const {
		std::ostringstream ss;
		ss << "{\"x\":" << x << ",\"y\":" << y << "}";
		return std::move(ss.str());
	}

	Vector2 Vector2::copy() const {
		return *this;
	}

	Vector2 Vector2::min(const Vector2& other) const {
		return Vector2(std::min(x, other.x), std::min(y, other.y));
	}

	Vector2 Vector2::max(const Vector2& other) const {
		return Vector2(std::max(x, other.x), std::max(y, other.y));
	}

	float Vector2::length() const {
		return sqrt(x * x + y * y);
	}

	bool Vector2::normalize() {
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

	Vector2 Vector2::normalized() const {
		auto copy = Vector2(x, y);
		copy.normalize();
		return copy;
	}
}
