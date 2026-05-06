#include "NIQuaternion.h"

#include "ExceptionUtil.h"

namespace NI {
	const Quaternion Quaternion::IDENTITY = { 1.0f, 0.0f, 0.0f, 0.0f };

	Quaternion::Quaternion() :
		w(0.0f),
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{

	}

	Quaternion::Quaternion(float _w, float _x, float _y, float _z) :
		w(_w),
		x(_x),
		y(_y),
		z(_z)
	{

	}

	Quaternion Quaternion::operator-() const {
		return { -w, -x, -y, -z };
	}

	Quaternion Quaternion::operator*(const Quaternion& q) const {
#if defined(SE_NI_QUATERNION_FNADDR_MULTIPLYQUATERNION) && SE_NI_QUATERNION_FNADDR_MULTIPLYQUATERNION > 0
		const auto NI_Quaternion_multiplyQuaternion = reinterpret_cast<Quaternion * (__thiscall*)(const Quaternion*, Quaternion*, const Quaternion*)>(SE_NI_QUATERNION_FNADDR_MULTIPLYQUATERNION);

		Quaternion result;
		NI_Quaternion_multiplyQuaternion(this, &result, &q);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	std::ostream& operator<<(std::ostream& str, const Quaternion& q) {
		str << "{" << q.w << "," << q.x << "," << q.y << "," << q.z << "}";
		return str;
	}

	std::string Quaternion::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Quaternion::toJson() const {
		std::ostringstream ss;
		ss << "{\"w\":" << w << ",\"x\":" << x << ",\"y\":" << y << ",\"z\":" << z << "}";
		return std::move(ss.str());
	}

	Quaternion Quaternion::copy() const {
		return Quaternion(w, x, y, z);
	}

	Quaternion Quaternion::invert() const {
#if defined(SE_NI_QUATERNION_FNADDR_INVERT) && SE_NI_QUATERNION_FNADDR_INVERT > 0
		const auto NI_Quaternion_UnitInverse = reinterpret_cast<Quaternion * (__cdecl*)(Quaternion*, const Quaternion*)>(SE_NI_QUATERNION_FNADDR_INVERT);

		Quaternion result;
		NI_Quaternion_UnitInverse(&result, this);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	Quaternion Quaternion::exp() const {
#if defined(SE_NI_QUATERNION_FNADDR_EXP) && SE_NI_QUATERNION_FNADDR_EXP > 0
		const auto NI_Quaternion_Exp = reinterpret_cast<Quaternion*(__cdecl*)(Quaternion*, const Quaternion*)>(SE_NI_QUATERNION_FNADDR_EXP);
		Quaternion result;
		NI_Quaternion_Exp(&result, this);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	Quaternion Quaternion::log() const {
#if defined(SE_NI_QUATERNION_FNADDR_LOG) && SE_NI_QUATERNION_FNADDR_LOG > 0
		const auto NI_Quaternion_Log = reinterpret_cast<Quaternion*(__cdecl*)(Quaternion*, const Quaternion*)>(SE_NI_QUATERNION_FNADDR_LOG);
		Quaternion result;
		NI_Quaternion_Log(&result, this);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	double Quaternion::dot(const Quaternion* q) const {
		// double precision as quaternions are sensitive to numerical issues.
		return double(w) * double(q->w)
			+ double(x) * double(q->x)
			+ double(y) * double(q->y)
			+ double(z) * double(q->z);
	}

	bool Quaternion::normalize() {
		double length = std::sqrt(this->dot(this));
		if (length > 1e-5) {
			w = float(w / length);
			x = float(x / length);
			y = float(y / length);
			z = float(z / length);
			return true;
		}
		else {
			w = x = y = z = 0.0;
			return false;
		}
	}

	Quaternion Quaternion::normalized() const {
		Quaternion result = *this;
		result.normalize();
		return result;
	}

	static Quaternion slerpBase(const Quaternion* q0, const Quaternion* q1, double dot_product, float t) {
		// Avoid using the vanilla implementation due to numerical issues.

		// Find (theta/2) while also handling values out of domain for acos.
		double half_theta;
		if (dot_product >= 1.0) {
			half_theta = 0;
		}
		else if (dot_product <= -1.0) {
			half_theta = 3.14159265358979;
		}
		else {
			half_theta = std::acos(dot_product);
		}

		double s = std::sin(half_theta);
		double k0, k1;
		if (s >= 1e-4) {
			// Slerp interpolation weights.
			double inv_s = 1.0 / s;
			k0 = inv_s * std::sin((1.0 - t) * half_theta);
			k1 = inv_s * std::sin(t * half_theta);
		}
		else {
			// Small angle approximation, numerically more stable by avoiding the 1/s term.
			// Vanilla slerp is missing this calculation.
			k0 = 1.0 - t;
			k1 = t;
		}

		return {
			float(k0 * q0->w + k1 * q1->w),
			float(k0 * q0->x + k1 * q1->x),
			float(k0 * q0->y + k1 * q1->y),
			float(k0 * q0->z + k1 * q1->z),
		};
	}

	Quaternion Quaternion::slerp(const Quaternion* q, float t) const {
		// Use shortest path interpolation.
		double dot_product = this->dot(q);
		Quaternion q_closest = (dot_product >= 0) ? *q : -*q;
		dot_product = std::abs(dot_product);

		return slerpBase(this, &q_closest, dot_product, t);
	}

	Quaternion Quaternion::slerpKeyframe(const Quaternion* q, float t) const {
		// Slerp with no checking for shortest path.
		double dot_product = this->dot(q);

		return slerpBase(this, q, dot_product, t);
	}

	Quaternion Quaternion::rotateTowards(const Quaternion* to, float rotationLimit) const {
		// Use shortest path inner product, clamp for numerical stability.
		double innerProduct = std::min(1.0, std::abs(this->dot(to)));
		double angle = 2.0 * std::acos(innerProduct);
		double t = 1.0;

		if (angle > 1e-5 && angle > rotationLimit) {
			t = std::max(0.0, std::min(1.0, rotationLimit / angle));
		}

		return slerp(to, float(t));
	}

	void Quaternion::fromAngleAxis(float angle, const NI::Vector3* axis) {
#if defined(SE_NI_QUATERNION_FNADDR_FROMANGLEAXIS) && SE_NI_QUATERNION_FNADDR_FROMANGLEAXIS > 0
		const auto NI_Quaternion_FromAngleAxis = reinterpret_cast<void(__thiscall*)(Quaternion*, float angle, const NI::Vector3 * axis)>(SE_NI_QUATERNION_FNADDR_FROMANGLEAXIS);

		NI_Quaternion_FromAngleAxis(this, angle, axis);

		// Normalize to avoid propagating numeric instability.
		normalize();
#else
		throw not_implemented_exception();
#endif
	}

	std::tuple<float, NI::Vector3> Quaternion::toAngleAxis() const {
#if defined(SE_NI_QUATERNION_FNADDR_TOANGLEAXIS) && SE_NI_QUATERNION_FNADDR_TOANGLEAXIS > 0
		const auto NI_Quaternion_ToAngleAxis = reinterpret_cast<void(__thiscall*)(const Quaternion*, float* angle, const NI::Vector3 * axis)>(SE_NI_QUATERNION_FNADDR_TOANGLEAXIS);

		float angle;
		NI::Vector3 axis;
		NI_Quaternion_ToAngleAxis(this, &angle, &axis);
		return std::make_tuple(angle, axis);
#else
		throw not_implemented_exception();
#endif
	}

	void Quaternion::fromRotation(const NI::Matrix33* rotation) {
#if defined(SE_NI_QUATERNION_FNADDR_FROMROTATION) && SE_NI_QUATERNION_FNADDR_FROMROTATION > 0
		const auto NI_Quaternion_FromRotation = reinterpret_cast<NI::Matrix33 * (__thiscall*)(Quaternion*, const NI::Matrix33*)>(SE_NI_QUATERNION_FNADDR_FROMROTATION);

		NI_Quaternion_FromRotation(this, rotation);

		// Normalize to avoid propagating numeric instability.
		normalize();
#else
		throw not_implemented_exception();
#endif
	}

	NI::Matrix33 Quaternion::toRotation() const {
		NI::Matrix33 result;
		result.fromQuaternion(this);
		return result;
	}
}
