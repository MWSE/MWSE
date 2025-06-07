#pragma once

#include "NIDefines.h"
#include "TES3Defines.h"

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

		void fromAngleAxis(float angle, const TES3::Vector3* axis);
		std::tuple<float, TES3::Vector3> toAngleAxis() const;

		void fromRotation(const TES3::Matrix33* rotation);
		TES3::Matrix33 toRotation() const;

		const static Quaternion IDENTITY;
	};
	static_assert(sizeof(Quaternion) == 0x10, "NI::Quaternion failed size validation");
}