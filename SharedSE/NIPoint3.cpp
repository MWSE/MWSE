#include "NIPoint3.h"

#include "NIColor.h"
#include "NIMatrix33.h"

namespace NI {
	const Point3 Point3::UNIT_X = { 1.0f, 0.0f, 0.0f };
	const Point3 Point3::UNIT_NEG_X = { -1.0f, 0.0f, 0.0f };
	const Point3 Point3::UNIT_Y = { 0.0f, 1.0f, 0.0f };
	const Point3 Point3::UNIT_NEG_Y = { 0.0f, -1.0f, 0.0f };
	const Point3 Point3::UNIT_Z = { 0.0f, 0.0f, 1.0f };
	const Point3 Point3::UNIT_NEG_Z = { 0.0f, 0.0f, -1.0f };
	const Point3 Point3::ONES = { 1.0f, 1.0f, 1.0f };
	const Point3 Point3::ZEROES = { 0.0f, 0.0f, 0.0f };
	const Point3 Point3::MIN = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	const Point3 Point3::MAX = { FLT_MAX, FLT_MAX, FLT_MAX };

	Point3::Point3() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{

	}

	Point3::Point3(float _x, float _y, float _z) :
		x(_x),
		y(_y),
		z(_z)
	{

	}

	Point3::Point3(const NI::Color& color) {
		x = color.r;
		y = color.g;
		z = color.b;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Point3::Point3(const sol::table& table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
		z = table.get_or("z", table.get_or(3, 0.0f));
	}
#endif

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Point3::Point3(const sol::object& object) {
		if (object.is<NI::Color>()) {
			*this = NI::Color(object.as<NI::Color>());
		}
		else if (object.is<sol::table>()) {
			*this = NI::Color(object.as<sol::table>());
		}
		throw std::invalid_argument("Could not convert lua object to TES3Point3.");
	}
#endif

	Point3& Point3::operator=(const NI::Color& vector) {
		x = vector.r;
		y = vector.g;
		z = vector.b;
		return *this;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Point3& Point3::operator=(const sol::table& table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
		z = table.get_or("z", table.get_or(3, 0.0f));
		return *this;
	}

	Point3& Point3::operator=(const sol::object& object) {
		if (object.is<NI::Color>()) {
			*this = object.as<NI::Color>();
		}
		else if (object.is<sol::table>()) {
			*this = object.as<sol::table>();
		}
		else {
			throw std::invalid_argument("Could not convert lua object to NiColor.");
		}
		return *this;
	}
#endif

	bool Point3::operator==(const Point3& vec3) const {
		return x == vec3.x && y == vec3.y && z == vec3.z;
	}

	bool Point3::operator!=(const Point3& vec3) const {
		return x != vec3.x || y != vec3.y || z != vec3.z;
	}

	Point3 Point3::operator+(const Point3& vec3) const {
		return Point3(x + vec3.x, y + vec3.y, z + vec3.z);
	}

	Point3 Point3::operator+(const float value) const {
		return Point3(x + value, y + value, z + value);
	}

	Point3 Point3::operator-() const {
		return Point3(-x, -y, -z);
	}

	Point3 Point3::operator-(const Point3& vec3) const {
		return Point3(x - vec3.x, y - vec3.y, z - vec3.z);
	}

	Point3 Point3::operator-(const float value) const {
		return Point3(x - value, y - value, z - value);
	}

	Point3 Point3::operator*(const Point3& vec3) const {
		return Point3(x * vec3.x, y * vec3.y, z * vec3.z);
	}

	Point3 Point3::operator*(const float scalar) const {
		return Point3(x * scalar, y * scalar, z * scalar);
	}

	Point3 Point3::operator/(const float scalar) const {
		return Point3(x / scalar, y / scalar, z / scalar);
	}

	std::ostream& operator<<(std::ostream& str, const Point3& vector) {
		str << "(" << vector.x << "," << vector.y << "," << vector.z << ")";
		return str;
	}

	std::string Point3::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Point3::toJson() const {
		std::ostringstream ss;
		ss << "{\"x\":" << x << ",\"y\":" << y << ",\"z\":" << z << "}";
		return std::move(ss.str());
	}

	Point3 Point3::copy() const {
		return *this;
	}

	NI::Color Point3::toNiColor() const {
		return NI::Color(x, y, z);
	}

	Point3 Point3::min(const Point3& other) const {
		return Point3(std::min(x, other.x), std::min(y, other.y), std::min(z, other.z));
	}

	Point3 Point3::max(const Point3& other) const {
		return Point3(std::max(x, other.x), std::max(y, other.y), std::max(z, other.z));
	}

	Point3 Point3::crossProduct(const Point3* vec3) const {
		return Point3(y * vec3->z - z * vec3->y, z * vec3->x - vec3->z * x, x * vec3->y - y * vec3->x);
	}

	float Point3::dotProduct(const Point3* vec3) const {
		return vec3->z * z + vec3->y * y + vec3->x * x;
	}

	Matrix33 Point3::outerProduct(const Point3* vec3) const {
		return Matrix33(
			(x * vec3->x), (y * vec3->x), (z * vec3->x),
			(x * vec3->y), (y * vec3->y), (z * vec3->y),
			(x * vec3->z), (y * vec3->z), (z * vec3->z)
		);
	}


	Point3 Point3::lerp(const Point3& to, float transition) const {
		auto transA = 1.0f - transition;
		return Point3(x * transA + to.x * transition, y * transA + to.y * transition, z * transA + to.z * transition);
	}

	float Point3::heightDifference(const Point3* vec3) const {
		return fabs(z - vec3->z);
	}

	float Point3::distance(const Point3* vec3) const {
		float dx = x - vec3->x;
		float dy = y - vec3->y;
		float dz = z - vec3->z;
		return sqrt(dz * dz + dx * dx + dy * dy);
	}

	float Point3::distanceChebyshev(const Point3* vec3) const {
		float dx = fabs(x - vec3->x);
		float dy = fabs(y - vec3->y);
		float dz = fabs(z - vec3->z);
		return std::max(std::max(dx, dy), dz);
	}

	float Point3::distanceManhattan(const Point3* vec3) const {
		float dx = fabs(x - vec3->x);
		float dy = fabs(y - vec3->y);
		float dz = fabs(z - vec3->z);
		return dx + dy + dz;
	}

	float Point3::distanceXY(const Point3* vec3) const {
		float dx = x - vec3->x;
		float dy = y - vec3->y;
		return sqrt(dx * dx + dy * dy);
	}

	float Point3::angle(const Point3* v) const {
		return acosf(dotProduct(v) / (length() * v->length()));
	}

	float Point3::length() const {
		return sqrt(x * x + y * y + z * z);
	}

	void Point3::negate() {
		x = -x;
		y = -y;
		z = -z;
	}

	bool Point3::normalize() {
		float len = length();
		if (len > 0.0f) {
			x = x / len;
			y = y / len;
			z = z / len;
			return true;
		}
		x = 0;
		y = 0;
		z = 0;
		return false;
	}

	Point3 Point3::normalized() const {
		auto copy = Point3(x, y, z);
		copy.normalize();
		return copy;
	}

	Point3 Point3::interpolate(const Point3& targetPoint, const float distance) const {
		auto line = targetPoint - *this;
		if (line.normalize()) {
			return *this + (line * distance);
		}
		return Point3();
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	bool Point3::canConvertFrom(const sol::table& table) {
		sol::optional<float> x = table["x"];
		if (!x) x = table[1];
		sol::optional<float> y = table["y"];
		if (!y) y = table[2];
		sol::optional<float> z = table["z"];
		if (!z) z = table[3];
		return x && y && z;
	}
#endif
}
