#pragma once

#include "NIDefines.h"

namespace TES3 {
	struct Matrix33;

	template <typename T>
	struct Range {
		T min;
		T max;
	};
	static_assert(sizeof(Range<int>) == 0x8, "TES3::Range<int> failed size validation");

	struct Vector2 {
		float x;
		float y;

		Vector2();
		Vector2(float x, float y);
		Vector2(sol::table table);

		Vector2& operator=(const sol::table table);

		bool operator==(const Vector2& vector) const;
		bool operator!=(const Vector2& vector) const;
		Vector2 operator+(const Vector2&) const;
		Vector2 operator-(const Vector2&) const;
		Vector2 operator*(const Vector2&) const;
		Vector2 operator*(const float) const;

		friend std::ostream& operator<<(std::ostream& str, const Vector2& vector);
		std::string toString() const;
		std::string toJson() const;

		Vector2 copy() const;

		float length() const;
	};
	static_assert(sizeof(Vector2) == 0x8, "TES3::Vector2 failed size validation");

	struct Vector3 {
		float x;
		float y;
		float z;

		Vector3();
		Vector3(float x, float y, float z);
		Vector3(NI::Color& color);
		Vector3(sol::table table);
		Vector3(sol::object object);

		Vector3& operator=(const NI::Color& vector);
		Vector3& operator=(const sol::table table);
		Vector3& operator=(const sol::object object);

		bool operator==(const Vector3& vector) const;
		bool operator!=(const Vector3& vector) const;
		Vector3 operator+(const Vector3&) const;
		Vector3 operator-(const Vector3&) const;
		Vector3 operator*(const Vector3&) const;
		Vector3 operator*(const float) const;

		friend std::ostream& operator<<(std::ostream& str, const Vector3& vector);
		std::string toString() const;
		std::string toJson() const;

		//
		// Associated functions.
		//

		Vector3 copy() const;
		NI::Color toNiColor() const;

		Vector3 crossProduct(const Vector3*) const;
		float dotProduct(const Vector3*) const;
		Matrix33 outerProduct(const Vector3*) const;
		
		Vector3 lerp(const Vector3& to, float transition) const;
		float heightDifference(const Vector3*) const;
		float distance(const Vector3*) const;
		float angle(const Vector3*) const;
		float length() const;
		void negate();
		bool normalize();
		Vector3 normalized() const;
		Vector3 interpolate(const Vector3&, const float) const;

	};
	static_assert(sizeof(Vector3) == 0xC, "TES3::Vector3 failed size validation");

	struct Vector4 {
		float w;
		float x;
		float y;
		float z;

		Vector4();
		Vector4(float w, float x, float y, float z);

		bool operator==(const Vector4& other) const;
		bool operator!=(const Vector4& other) const;
		Vector4 operator+(const Vector4& other) const;
		Vector4 operator-(const Vector4& other) const;
		Vector4 operator*(const Vector4& other) const;
		Vector4 operator*(float scalar) const;

		friend std::ostream& operator<<(std::ostream& str, const Matrix33& matrix);
		std::string toString() const;
		std::string toJson() const;

		Vector4 copy() const;

		float length() const;
	};
	static_assert(sizeof(Vector4) == 0x10, "TES3::Vector4 failed size validation");

	struct Matrix33 {
		Vector3 m0;
		Vector3 m1;
		Vector3 m2;

		Matrix33();
		Matrix33(Vector3* m0, Vector3* m1, Vector3* m2);
		Matrix33(float m0x, float m0y, float m0z, float m1x, float m1y, float m1z, float m2x, float m2y, float m2z);

		//
		// Basic operators.
		//

		bool operator==(const Matrix33& matrix);
		bool operator!=(const Matrix33& matrix);
		Matrix33 operator+(const Matrix33& matrix);
		Matrix33 operator-(const Matrix33& matrix);
		Matrix33 operator*(const Matrix33& matrix);
		Vector3 operator*(const Vector3& vector);
		Matrix33 operator*(float scalar);

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

		//
		// Other related helper functions.
		//

		Matrix33 transpose();

		Matrix33 invert() const;
		bool invert(Matrix33* out_matrix) const;
		std::tuple<Matrix33, bool> invert_lua() const;

		void fromEulerXYZ(float x, float y, float z);
		bool toEulerXYZ(float* x, float* y, float* z) const;
		std::tuple<Vector3, bool> toEulerXYZ_lua() const;
		bool toEulerZYX(float* x, float* y, float* z) const;
		std::tuple<Vector3, bool> toEulerZYX_lua() const;

		bool reorthogonalize();

	};
	static_assert(sizeof(Matrix33) == 0x24, "TES3::Matrix33 failed size validation");

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

	};
	static_assert(sizeof(Matrix44) == 0x40, "TES3::Matrix44 failed size validation");

	struct BoundingBox {
		Vector3 minimum;
		Vector3 maximum;

		BoundingBox();
		BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

		bool operator==(const BoundingBox& other) const;
		bool operator!=(const BoundingBox& other) const;

		friend std::ostream& operator<<(std::ostream& str, const BoundingBox& other);
		std::string toString() const;
		std::string toJson() const;

		BoundingBox copy() const;
	};
	static_assert(sizeof(BoundingBox) == 0x18, "TES3::BoundingBox failed size validation");

	struct Transform {
		TES3::Matrix33 rotation;
		TES3::Vector3 translation;
		float scale;

		Transform copy() const;
	};
	static_assert(sizeof(Transform) == 0x34, "TES3::Transform failed size validation");
}
