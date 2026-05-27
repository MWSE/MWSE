#pragma once

#include "NIPoint3.h"

namespace NI {
	struct Matrix33 {
		Point3 m0;
		Point3 m1;
		Point3 m2;

		Matrix33();
		Matrix33(const Point3& outerA, const Point3& outerB);
		Matrix33(Point3* m0, Point3* m1, Point3* m2);
		Matrix33(float m0x, float m0y, float m0z, float m1x, float m1y, float m1z, float m2x, float m2y, float m2z);
		Matrix33(const Quaternion& fromQuaternion);

		//
		// Basic operators.
		//

		bool operator==(const Matrix33& matrix) const;
		bool operator!=(const Matrix33& matrix) const;
		Matrix33 operator+(const Matrix33& matrix) const;
		Matrix33 operator-(const Matrix33& matrix) const;
		Matrix33 operator*(const Matrix33& matrix) const;
		Point3 operator*(const Point3& vector) const;
		Matrix33 operator*(float scalar) const;

		friend std::ostream& operator<<(std::ostream& str, const Matrix33& matrix);
		std::string toString() const;
		std::string toJson() const;

		//
		// Set the matrix to specific useful values.
		//

		Matrix33 copy() const;

		void toZero();
		void toIdentity();
		void toRotationX(float x);
		void toRotationY(float y);
		void toRotationZ(float z);
		void toRotation(float angle, float x, float y, float z);
		void toRotation(float angle, const Point3& axis);
		bool toRotationDifference(const Point3& a, const Point3& b);

		//
		// Other related helper functions.
		//

		Matrix33 transpose();

		Matrix33 invert() const;
		bool invert(Matrix33* out_matrix) const;
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		std::tuple<Matrix33, bool> invert_lua() const;
#endif

		void fromEulerXYZ(float x, float y, float z);
		bool toEulerXYZ(Point3* vector) const;
		bool toEulerXYZ(float* x, float* y, float* z) const;
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		std::tuple<Point3, bool> toEulerXYZ_lua() const;
#endif

		bool toEulerZYX(Point3* vector) const;
		bool toEulerZYX(float* x, float* y, float* z) const;
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		std::tuple<Point3, bool> toEulerZYX_lua() const;
#endif

		NI::Quaternion toQuaternion() const;

		Point3 getForwardVector() const;
		Point3 getRightVector() const;
		Point3 getUpVector() const;

		void lookAt(const Point3& direction, const Point3& worldUp);

		void fromQuaternion(const Quaternion* quaternion);

		bool reorthogonalize();

		const static Matrix33 IDENTITY;

	};
	static_assert(sizeof(Matrix33) == 0x24, "NI::Matrix33 failed size validation");
}
