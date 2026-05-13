#pragma once

namespace NI {
	struct Matrix33;

	struct Point4 {
		float x; // 0x0
		float y; // 0x4
		float z; // 0x8
		float w; // 0xC

		Point4();
		Point4(float x, float y, float z, float w);

		bool operator==(const Point4& other) const;
		bool operator!=(const Point4& other) const;
		Point4 operator+(const Point4& other) const;
		Point4 operator-(const Point4& other) const;
		Point4 operator*(const Point4& other) const;
		Point4 operator*(const float scalar) const;
		Point4 operator/(const float scalar) const;

		friend std::ostream& operator<<(std::ostream& str, const Point4& matrix);
		std::string toString() const;
		std::string toJson() const;

		Point4 copy() const;
		Point4 min(const Point4& other) const;
		Point4 max(const Point4& other) const;

		float distance(const Point4*) const;
		float distanceChebyshev(const Point4*) const;
		float distanceManhattan(const Point4*) const;

		float length() const;
	};
}