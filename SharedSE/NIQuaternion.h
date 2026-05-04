#pragma once

#include "NIDefines.h"
#include "NIMatrix33.h"
#include "NIVector3.h"

namespace NI {
	struct Quaternion {
		float w;
		float x;
		float y;
		float z;

		Quaternion();
		Quaternion(float w, float x, float y, float z);

		Quaternion operator-() const;
		Quaternion operator*(const Quaternion& q) const;

		friend std::ostream& operator<<(std::ostream& str, const Quaternion& q);
		std::string toString() const;
		std::string toJson() const;

		Quaternion copy() const;
		Quaternion invert() const;
		Quaternion exp() const;
		Quaternion log() const;
		double dot(const Quaternion* q) const;
		bool normalize();
		Quaternion normalized() const;

		Quaternion slerp(const Quaternion* q, float t) const;
		Quaternion slerpKeyframe(const Quaternion* q, float t) const;
		Quaternion rotateTowards(const Quaternion* to, float rotationLimit) const;

		void fromAngleAxis(float angle, const Vector3* axis);
		std::tuple<float, Vector3> toAngleAxis() const;

		void fromRotation(const Matrix33* rotation);
		Matrix33 toRotation() const;

		const static Quaternion IDENTITY;
	};
	static_assert(sizeof(Quaternion) == 0x10, "NI::Quaternion failed size validation");
}
