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

		Quaternion operator*(const Quaternion& q) const;

		friend std::ostream& operator<<(std::ostream& str, const Quaternion& q);
		std::string toString() const;
		std::string toJson() const;

		Quaternion copy() const;
		Quaternion invert() const;
		Quaternion slerp(const Quaternion* q, float t) const;

		void fromAngleAxis(float angle, const TES3::Vector3* axis);
		std::tuple<float, TES3::Vector3> toAngleAxis() const;

		void fromRotation(const TES3::Matrix33* rotation);
		TES3::Matrix33 toRotation() const;
	};
	static_assert(sizeof(Quaternion) == 0x10, "NI::Quaternion failed size validation");
}