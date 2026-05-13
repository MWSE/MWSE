#include "NIPoint4.h"

namespace NI {
	Point4::Point4() :
		x(0.0f),
		y(0.0f),
		z(0.0f),
		w(0.0f)
	{

	}

	Point4::Point4(float _x, float _y, float _z, float _w) :
		x(_x),
		y(_y),
		z(_z),
		w(_w)
	{

	}

	bool Point4::operator==(const Point4& other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool Point4::operator!=(const Point4& other) const {
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}

	Point4 Point4::operator+(const Point4& other) const {
		return Point4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Point4 Point4::operator-(const Point4& other) const {
		return Point4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	Point4 Point4::operator*(const Point4& other) const {
		return Point4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	Point4 Point4::operator*(const float scalar) const {
		return Point4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	Point4 Point4::operator/(const float scalar) const {
		return Point4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	std::ostream& operator<<(std::ostream& str, const Point4& vector) {
		str << "(" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << ")";
		return str;
	}

	std::string Point4::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Point4::toJson() const {
		std::ostringstream ss;
		ss << "{\"x\":" << x << ",\"y\":" << y << ",\"z\":" << z << ",\"w\":" << w << "}";
		return std::move(ss.str());
	}

	Point4 Point4::copy() const {
		return *this;
	}

	Point4 Point4::min(const Point4& other) const {
		return Point4(std::min(x, other.x), std::min(y, other.y), std::min(z, other.z), std::min(w, other.w));
	}

	Point4 Point4::max(const Point4& other) const {
		return Point4(std::max(x, other.x), std::max(y, other.y), std::max(z, other.z), std::max(w, other.w));
	}

	float Point4::distance(const Point4* vec4) const {
		float dx = x - vec4->x;
		float dy = y - vec4->y;
		float dz = z - vec4->z;
		float dw = w - vec4->w;
		return sqrt(dz * dz + dw * dw + dx * dx + dy * dy);
	}

	float Point4::distanceChebyshev(const Point4* vec4) const {
		float dx = fabs(x - vec4->x);
		float dy = fabs(y - vec4->y);
		float dz = fabs(z - vec4->z);
		float dw = fabs(w - vec4->w);
		return std::max(std::max(dx, dy), std::max(dz, dw));
	}

	float Point4::distanceManhattan(const Point4* vec4) const {
		float dx = fabs(x - vec4->x);
		float dy = fabs(y - vec4->y);
		float dz = fabs(z - vec4->z);
		float dw = fabs(w - vec4->w);
		return dx + dy + dz + dw;
	}

	float Point4::length() const {
		return sqrt(x * x + y * y + z * z + w * w);
	}
}
