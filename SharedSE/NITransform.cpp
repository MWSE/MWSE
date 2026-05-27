#include "NITransform.h"

namespace NI {
	Transform::Transform() {
		this->toIdentity();
	}

	Transform::Transform(const Matrix33& rotation, const Point3& translation, const float scale) :
		rotation(rotation),
		translation(translation),
		scale(scale)
	{
	}

	Transform Transform::operator*(const Transform& transform) const {
		return {
			rotation * transform.rotation,
			rotation * transform.translation * scale + translation,
			scale * transform.scale
		};
	}

	Point3 Transform::operator*(const Point3& vector) const {
		return rotation * vector * scale + translation;
	}

	bool Transform::invert(Transform* out) const {
		if (scale == 0.0f) {
			return false;
		}
		bool success = rotation.invert(&out->rotation);
		if (!success) {
			return false;
		}
		out->scale = 1.0f / scale;
		out->translation = -(out->rotation * translation * out->scale);
		return true;
	}

	std::tuple<Transform, bool> Transform::invert() const {
		auto transform = Transform();
		bool valid = invert(&transform);
		return std::make_tuple(transform, valid);
	}

	Transform Transform::copy() const {
		return *this;
	}

	void Transform::toIdentity() {
		rotation.toIdentity();
		translation = Point3::ZEROES;
		scale = 1.0f;
	}
}
