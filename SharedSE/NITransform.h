#pragma once

#include "NIMatrix33.h"

namespace NI {
	struct Transform {
		Matrix33 rotation;
		Vector3 translation;
		float scale;

		Transform();
		Transform(const Matrix33& rotation, const Vector3& translation, const float scale);

		Transform operator*(const Transform& transform) const;
		Vector3 operator*(const Vector3& transform) const;

		bool invert(Transform* out) const;
		std::tuple<Transform, bool> invert() const;

		Transform copy() const;
		void toIdentity();
	};
	static_assert(sizeof(Transform) == 0x34, "NI::Transform failed size validation");
}
