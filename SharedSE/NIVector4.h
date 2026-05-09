#pragma once

namespace NI {
	struct Matrix33;

	struct Vector4 {
		float x; // 0x0
		float y; // 0x4
		float z; // 0x8
		float w; // 0xC

		Vector4();
		Vector4(float x, float y, float z, float w);

		bool operator==(const Vector4& other) const;
		bool operator!=(const Vector4& other) const;
		Vector4 operator+(const Vector4& other) const;
		Vector4 operator-(const Vector4& other) const;
		Vector4 operator*(const Vector4& other) const;
		Vector4 operator*(const float scalar) const;
		Vector4 operator/(const float scalar) const;

		friend std::ostream& operator<<(std::ostream& str, const Vector4& matrix);
		std::string toString() const;
		std::string toJson() const;

		Vector4 copy() const;
		Vector4 min(const Vector4& other) const;
		Vector4 max(const Vector4& other) const;

		float distance(const Vector4*) const;
		float distanceChebyshev(const Vector4*) const;
		float distanceManhattan(const Vector4*) const;

		float length() const;
	};
}