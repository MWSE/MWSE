#pragma once

#include "NIVector4.h"

namespace NI {
	struct Matrix44 {
		Vector4 m0;
		Vector4 m1;
		Vector4 m2;
		Vector4 m3;

		Matrix44();
		Matrix44(const Vector4& m0, const Vector4& m1, const Vector4& m2, const Vector4& m3);
		Matrix44(float m0w, float m0x, float m0y, float m0z, float m1w, float m1x, float m1y, float m1z, float m2w, float m2x, float m2y, float m2z, float m3w, float m3x, float m3y, float m3z);

		//
		// Basic operators.
		//

		bool operator==(const Matrix44& matrix);
		bool operator!=(const Matrix44& matrix);
		Matrix44 operator+(const Matrix44& matrix);
		Matrix44 operator-(const Matrix44& matrix);
		Matrix44 operator*(const Matrix44& matrix);
		Matrix44 operator*(float scalar);

		friend std::ostream& operator<<(std::ostream& str, const Matrix44& matrix);
		std::string toString() const;
		std::string toJson() const;

		//
		// Set the matrix to specific useful values.
		//

		Matrix44 copy() const;

		void toZero();

		const static Matrix44 IDENTITY;
	};
	static_assert(sizeof(Matrix44) == 0x40, "TES3::Matrix44 failed size validation");
}
