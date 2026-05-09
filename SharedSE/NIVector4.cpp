#include "NIVector4.h"

namespace NI {
	Vector4::Vector4() :
		x(0.0f),
		y(0.0f),
		z(0.0f),
		w(0.0f)
	{

	}

	Vector4::Vector4(float _x, float _y, float _z, float _w) :
		x(_x),
		y(_y),
		z(_z),
		w(_w)
	{

	}

	bool Vector4::operator==(const Vector4& other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool Vector4::operator!=(const Vector4& other) const {
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}

	Vector4 Vector4::operator+(const Vector4& other) const {
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vector4 Vector4::operator-(const Vector4& other) const {
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	Vector4 Vector4::operator*(const Vector4& other) const {
		return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	Vector4 Vector4::operator*(const float scalar) const {
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	Vector4 Vector4::operator/(const float scalar) const {
		return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	std::ostream& operator<<(std::ostream& str, const Vector4& vector) {
		str << "(" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << ")";
		return str;
	}

	std::string Vector4::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Vector4::toJson() const {
		std::ostringstream ss;
		ss << "{\"x\":" << x << ",\"y\":" << y << ",\"z\":" << z << ",\"w\":" << w << "}";
		return std::move(ss.str());
	}

	Vector4 Vector4::copy() const {
		return *this;
	}

	Vector4 Vector4::min(const Vector4& other) const {
		return Vector4(std::min(x, other.x), std::min(y, other.y), std::min(z, other.z), std::min(w, other.w));
	}

	Vector4 Vector4::max(const Vector4& other) const {
		return Vector4(std::max(x, other.x), std::max(y, other.y), std::max(z, other.z), std::max(w, other.w));
	}

	float Vector4::distance(const Vector4* vec4) const {
		float dx = x - vec4->x;
		float dy = y - vec4->y;
		float dz = z - vec4->z;
		float dw = w - vec4->w;
		return sqrt(dz * dz + dw * dw + dx * dx + dy * dy);
	}

	float Vector4::distanceChebyshev(const Vector4* vec4) const {
		float dx = fabs(x - vec4->x);
		float dy = fabs(y - vec4->y);
		float dz = fabs(z - vec4->z);
		float dw = fabs(w - vec4->w);
		return std::max(std::max(dx, dy), std::max(dz, dw));
	}

	float Vector4::distanceManhattan(const Vector4* vec4) const {
		float dx = fabs(x - vec4->x);
		float dy = fabs(y - vec4->y);
		float dz = fabs(z - vec4->z);
		float dw = fabs(w - vec4->w);
		return dx + dy + dz + dw;
	}

	float Vector4::length() const {
		return sqrt(x * x + y * y + z * z + w * w);
	}
}
