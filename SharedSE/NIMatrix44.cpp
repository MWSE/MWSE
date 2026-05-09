#include "NIMatrix44.h"

namespace NI {
	const Matrix44 Matrix44::IDENTITY = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	Matrix44::Matrix44() :
		m0(),
		m1(),
		m2(),
		m3()
	{

	}

	Matrix44::Matrix44(const Vector4& in_m0, const Vector4& in_m1, const Vector4& in_m2, const Vector4& in_m3) :
		m0(in_m0),
		m1(in_m1),
		m2(in_m2),
		m3(in_m3)
	{

	}

	Matrix44::Matrix44(float m0w, float m0x, float m0y, float m0z, float m1w, float m1x, float m1y, float m1z, float m2w, float m2x, float m2y, float m2z, float m3w, float m3x, float m3y, float m3z) :
		m0(m0w, m0x, m0y, m0z),
		m1(m1w, m1x, m1y, m1z),
		m2(m2w, m2x, m2y, m2z),
		m3(m3w, m3x, m3y, m3z)
	{

	}

	bool Matrix44::operator==(const Matrix44& matrix) {
		return m0 == matrix.m0 && m1 == matrix.m1 && m2 == matrix.m2 && m3 == matrix.m3;
	}

	bool Matrix44::operator!=(const Matrix44& matrix) {
		return m0 != matrix.m0 || m1 != matrix.m1 || m2 != matrix.m2 || m3 != matrix.m3;
	}

	Matrix44 Matrix44::operator+(const Matrix44& matrix) {
		return Matrix44(m0 + matrix.m0, m1 + matrix.m1, m2 + matrix.m2, m3 + matrix.m3);
	}

	Matrix44 Matrix44::operator-(const Matrix44& matrix) {
		return Matrix44(m0 - matrix.m0, m1 - matrix.m1, m2 - matrix.m2, m3 - matrix.m3);
	}

	Matrix44 Matrix44::operator*(const Matrix44& matrix) {
		return Matrix44(
			m0.w * matrix.m0.w + m0.x * matrix.m1.w + m0.y * matrix.m2.w + m0.z * matrix.m3.w,
			m0.w * matrix.m0.x + m0.x * matrix.m1.x + m0.y * matrix.m2.x + m0.z * matrix.m3.x,
			m0.w * matrix.m0.y + m0.x * matrix.m1.y + m0.y * matrix.m2.y + m0.z * matrix.m3.y,
			m0.w * matrix.m0.z + m0.x * matrix.m1.z + m0.y * matrix.m2.z + m0.z * matrix.m3.z,

			m1.w * matrix.m0.w + m1.x * matrix.m1.w + m1.y * matrix.m2.w + m1.z * matrix.m3.w,
			m1.w * matrix.m0.x + m1.x * matrix.m1.x + m1.y * matrix.m2.x + m1.z * matrix.m3.x,
			m1.w * matrix.m0.y + m1.x * matrix.m1.y + m1.y * matrix.m2.y + m1.z * matrix.m3.y,
			m1.w * matrix.m0.z + m1.x * matrix.m1.z + m1.y * matrix.m2.z + m1.z * matrix.m3.z,

			m2.w * matrix.m0.w + m2.x * matrix.m1.w + m2.y * matrix.m2.w + m2.z * matrix.m3.w,
			m2.w * matrix.m0.x + m2.x * matrix.m1.x + m2.y * matrix.m2.x + m2.z * matrix.m3.x,
			m2.w * matrix.m0.y + m2.x * matrix.m1.y + m2.y * matrix.m2.y + m2.z * matrix.m3.y,
			m2.w * matrix.m0.z + m2.x * matrix.m1.z + m2.y * matrix.m2.z + m2.z * matrix.m3.z,

			m3.w * matrix.m0.w + m3.x * matrix.m1.w + m3.y * matrix.m2.w + m3.z * matrix.m3.w,
			m3.w * matrix.m0.x + m3.x * matrix.m1.x + m3.y * matrix.m2.x + m3.z * matrix.m3.x,
			m3.w * matrix.m0.y + m3.x * matrix.m1.y + m3.y * matrix.m2.y + m3.z * matrix.m3.y,
			m3.w * matrix.m0.z + m3.x * matrix.m1.z + m3.y * matrix.m2.z + m3.z * matrix.m3.z
		);
	}

	Matrix44 Matrix44::operator*(float scalar) {
		return Matrix44(m0 * scalar, m1 * scalar, m2 * scalar, m3 * scalar);
	}

	std::ostream& operator<<(std::ostream& str, const Matrix44& matrix) {
		str << "[" << matrix.m0 << "," << matrix.m1 << "," << matrix.m2 << "," << matrix.m3 << "]";
		return str;
	}

	std::string Matrix44::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Matrix44::toJson() const {
		std::ostringstream ss;
		ss << "["
			<< m0.toJson() << ","
			<< m1.toJson() << ","
			<< m2.toJson() << ","
			<< m3.toJson()
			<< "]";
		return std::move(ss.str());
	}

	Matrix44 Matrix44::copy() const {
		return *this;
	}

	void Matrix44::toZero() {
		m0.w = 0.0f;
		m0.x = 0.0f;
		m0.y = 0.0f;
		m0.z = 0.0f;
		m1.w = 0.0f;
		m1.x = 0.0f;
		m1.y = 0.0f;
		m1.z = 0.0f;
		m2.w = 0.0f;
		m2.x = 0.0f;
		m2.y = 0.0f;
		m2.z = 0.0f;
		m3.w = 0.0f;
		m3.x = 0.0f;
		m3.y = 0.0f;
		m3.z = 0.0f;
	}
}
