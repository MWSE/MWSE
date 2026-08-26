#pragma once

#include "NIMatrix33.h"

namespace NI {
	struct Transform {
		Matrix33 rotation;
		Point3 translation;
		float scale;

		Transform();
		Transform(const Matrix33& rotation, const Point3& translation, const float scale);

		bool operator==(const Transform& transform) const;
		bool operator!=(const Transform& transform) const;
		Transform operator*(const Transform& transform) const;
		Point3 operator*(const Point3& transform) const;

		bool invert(Transform* out) const;
		std::tuple<Transform, bool> invert() const;

		Transform copy() const;
		void toIdentity();

		const static Transform IDENTITY;
	};
	static_assert(sizeof(Transform) == 0x34, "NI::Transform failed size validation");
}
