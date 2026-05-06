#include "NIMatrix33.h"

#include "ExceptionUtil.h"
#include "MathUtil.h"

namespace NI {
	Matrix33::Matrix33() : m0(), m1(), m2() {

	}

	Matrix33::Matrix33(const Vector3& outerA, const Vector3& outerB) : Matrix33(
		(outerA.x* outerB.x), (outerA.y* outerB.x), (outerA.z* outerB.x),
		(outerA.x* outerB.y), (outerA.y* outerB.y), (outerA.z* outerB.y),
		(outerA.x* outerB.z), (outerA.y* outerB.z), (outerA.z* outerB.z)
	) {

	}

	Matrix33::Matrix33(Vector3* in_m0, Vector3* in_m1, Vector3* in_m2) {
		m0 = *in_m0;
		m1 = *in_m1;
		m2 = *in_m2;
	}

	Matrix33::Matrix33(float m0x, float m0y, float m0z, float m1x, float m1y, float m1z, float m2x, float m2y, float m2z) {
		m0.x = m0x;
		m0.y = m0y;
		m0.z = m0z;
		m1.x = m1x;
		m1.y = m1y;
		m1.z = m1z;
		m2.x = m2x;
		m2.y = m2y;
		m2.z = m2z;
	}

	Matrix33::Matrix33(const Quaternion& quaternion) {
#if defined(SE_NI_MATRIX33_FNADDR_CTOR_FROMQUATERNION) && SE_NI_MATRIX33_FNADDR_CTOR_FROMQUATERNION > 0
		const auto NI_Matrix33_ctor_fromQuaternion = reinterpret_cast<Matrix33 * (__thiscall*)(Matrix33*, const Quaternion*)>(SE_NI_MATRIX33_FNADDR_CTOR_FROMQUATERNION);
		return NI_Matrix33_ctor_fromQuaternion(this, &quaternion);
#else
		throw not_implemented_exception();
#endif
	}

	void Matrix33::fromQuaternion(const Quaternion* quaternion) {
#if defined(SE_NI_MATRIX33_FNADDR_CTOR_FROMQUATERNION) && SE_NI_MATRIX33_FNADDR_CTOR_FROMQUATERNION > 0
		const auto NI_Matrix33_ctor_fromQuaternion = reinterpret_cast<Matrix33 * (__thiscall*)(Matrix33*, const Quaternion*)>(SE_NI_MATRIX33_FNADDR_CTOR_FROMQUATERNION);
		NI_Matrix33_ctor_fromQuaternion(this, quaternion);
#else
		throw not_implemented_exception();
#endif
	}

	bool Matrix33::operator==(const Matrix33& matrix) const {
#if defined(SE_NI_MATRIX33_FNADDR_TESTEQUAL) && SE_NI_MATRIX33_FNADDR_TESTEQUAL > 0
		const auto NI_Matrix33_testEqual = reinterpret_cast<bool(__thiscall*)(Matrix33*, const Matrix33*)>(SE_NI_MATRIX33_FNADDR_TESTEQUAL);
		return NI_Matrix33_testEqual(this, &matrix);
#else
		return m0 == matrix.m0 && m1 == matrix.m1 && m2 == matrix.m2;
#endif
	}

	bool Matrix33::operator!=(const Matrix33& matrix) const {
		return m0.x != matrix.m0.x
			|| m0.y != matrix.m0.y
			|| m0.z != matrix.m0.z
			|| m1.x != matrix.m1.x
			|| m1.y != matrix.m1.y
			|| m1.z != matrix.m1.z
			|| m2.x != matrix.m2.x
			|| m2.y != matrix.m2.y
			|| m2.z != matrix.m2.z;
	}

	Matrix33 Matrix33::operator+(const Matrix33& matrix) const {
#if defined(SE_NI_MATRIX33_FNADDR_ADDMATRIX) && SE_NI_MATRIX33_FNADDR_ADDMATRIX > 0
		const auto NI_Matrix33_addMatrix = reinterpret_cast<Matrix33 * (__thiscall*)(const Matrix33*, Matrix33*, const Matrix33*)>(SE_NI_MATRIX33_FNADDR_ADDMATRIX);

		Matrix33 result;
		NI_Matrix33_addMatrix(this, &result, &matrix);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	Matrix33 Matrix33::operator-(const Matrix33& matrix) const {
#if defined(SE_NI_MATRIX33_FNADDR_SUBTRACTMATRIX) && SE_NI_MATRIX33_FNADDR_SUBTRACTMATRIX > 0
		const auto NI_Matrix33_subtractMatrix = reinterpret_cast<Matrix33 * (__thiscall*)(const Matrix33*, Matrix33*, const Matrix33*)>(SE_NI_MATRIX33_FNADDR_SUBTRACTMATRIX);

		Matrix33 result;
		NI_Matrix33_subtractMatrix(this, &result, &matrix);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	Matrix33 Matrix33::operator*(const Matrix33& matrix) const {
#if defined(SE_NI_MATRIX33_FNADDR_MULTIPLYMATRIX) && SE_NI_MATRIX33_FNADDR_MULTIPLYMATRIX > 0
		const auto NI_Matrix33_multiplyMatrix = reinterpret_cast<Matrix33 * (__thiscall*)(const Matrix33*, Matrix33*, const Matrix33*)>(SE_NI_MATRIX33_FNADDR_MULTIPLYMATRIX);

		Matrix33 result;
		NI_Matrix33_multiplyMatrix(this, &result, &matrix);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	Vector3 Matrix33::operator*(const Vector3& vector) const {
#if defined(SE_NI_MATRIX33_FNADDR_MULTIPLYVECTOR) && SE_NI_MATRIX33_FNADDR_MULTIPLYVECTOR > 0
		const auto NI_Matrix33_multiplyVector = reinterpret_cast<Vector3 * (__thiscall*)(const Matrix33*, Vector3*, const Vector3*)>(SE_NI_MATRIX33_FNADDR_MULTIPLYVECTOR);

		Vector3 result;
		NI_Matrix33_multiplyVector(this, &result, &vector);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	Matrix33 Matrix33::operator*(float scalar) const {
#if defined(SE_NI_MATRIX33_FNADDR_MULTIPLYSCALAR) && SE_NI_MATRIX33_FNADDR_MULTIPLYSCALAR > 0
		const auto NI_Matrix33_multiplyScalar = reinterpret_cast<Matrix33 * (__thiscall*)(const Matrix33*, Matrix33*, float)>(SE_NI_MATRIX33_FNADDR_MULTIPLYSCALAR);

		Matrix33 result;
		NI_Matrix33_multiplyScalar(this, &result, scalar);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	std::ostream& operator<<(std::ostream& str, const Matrix33& matrix) {
		str << "[" << matrix.m0 << "," << matrix.m1 << "," << matrix.m2 << "]";
		return str;
	}

	std::string Matrix33::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Matrix33::toJson() const {
		std::ostringstream ss;
		ss << "["
			<< "[" << m0.x << "," << m0.y << "," << m0.z << "],"
			<< "[" << m1.x << "," << m1.y << "," << m1.z << "],"
			<< "[" << m2.x << "," << m2.y << "," << m2.z << "]"
			<< "]";
		return std::move(ss.str());
	}

	Matrix33 Matrix33::copy() const {
		return *this;
	}

	void Matrix33::toZero() {
		m0.x = 0.0f;
		m0.y = 0.0f;
		m0.z = 0.0f;
		m1.x = 0.0f;
		m1.y = 0.0f;
		m1.z = 0.0f;
		m2.x = 0.0f;
		m2.y = 0.0f;
		m2.z = 0.0f;
	}

	void Matrix33::toIdentity() {
#if defined(SE_NI_MATRIX33_FNADDR_TOIDENTITY) && SE_NI_MATRIX33_FNADDR_TOIDENTITY > 0
		const auto NI_Matrix33_toIdentity = reinterpret_cast<void(__thiscall*)(Matrix33*)>(SE_NI_MATRIX33_FNADDR_TOIDENTITY);

		NI_Matrix33_toIdentity(this);
#else
		throw not_implemented_exception();
#endif
	}

	void Matrix33::toRotationX(float value) {
#if defined(SE_NI_MATRIX33_FNADDR_TOROTATIONX) && SE_NI_MATRIX33_FNADDR_TOROTATIONX > 0
		const auto NI_Matrix33_toRotationX = reinterpret_cast<void(__thiscall*)(Matrix33*, float)>(SE_NI_MATRIX33_FNADDR_TOROTATIONX);

		NI_Matrix33_toRotationX(this, value);
#else
		throw not_implemented_exception();
#endif
	}

	void Matrix33::toRotationY(float value) {
#if defined(SE_NI_MATRIX33_FNADDR_TOROTATIONY) && SE_NI_MATRIX33_FNADDR_TOROTATIONY > 0
		const auto NI_Matrix33_toRotationY = reinterpret_cast<void(__thiscall*)(Matrix33*, float)>(SE_NI_MATRIX33_FNADDR_TOROTATIONY);

		NI_Matrix33_toRotationY(this, value);
#else
		throw not_implemented_exception();
#endif
	}

	void Matrix33::toRotationZ(float value) {
#if defined(SE_NI_MATRIX33_FNADDR_TOROTATIONZ) && SE_NI_MATRIX33_FNADDR_TOROTATIONZ > 0
		const auto NI_Matrix33_toRotationZ = reinterpret_cast<void(__thiscall*)(Matrix33*, float)>(SE_NI_MATRIX33_FNADDR_TOROTATIONZ);

		NI_Matrix33_toRotationZ(this, value);
#else
		throw not_implemented_exception();
#endif
	}

	void Matrix33::toRotation(float angle, const Vector3& axis) {
#if defined(SE_NI_MATRIX33_FNADDR_TOROTATIONXYZ) && SE_NI_MATRIX33_FNADDR_TOROTATIONXYZ > 0
		const auto NI_Matrix33_toRotationXYZ = reinterpret_cast<void(__thiscall*)(Matrix33*, float, float, float, float)>(SE_NI_MATRIX33_FNADDR_TOROTATIONXYZ);

		NI_Matrix33_toRotationXYZ(this, angle, axis.x, axis.y, axis.z);
#else
		throw not_implemented_exception();
#endif
	}

	bool Matrix33::toRotationDifference(const Vector3& a, const Vector3& b) {
		using se::math::M_PIf;

		auto axis = a.crossProduct(&b);
		auto norm = axis.length();
		if (norm <= 1e-5) {
			toIdentity();
			return false;
		}
		else {
			axis = axis / norm;
			auto angle = asin(norm);
			if (a.dotProduct(&b) < 0) {
				angle = M_PIf - angle;
			}
			toRotation(-angle, axis);
			return true;
		}
	}

	void Matrix33::fromEulerXYZ(float x, float y, float z) {
#if defined(SE_NI_MATRIX33_FNADDR_FROMEULERXYZ) && SE_NI_MATRIX33_FNADDR_FROMEULERXYZ > 0
		const auto NI_Matrix33_fromEulerXYZ = reinterpret_cast<void(__thiscall*)(Matrix33*, float, float, float)>(SE_NI_MATRIX33_FNADDR_FROMEULERXYZ);

		NI_Matrix33_fromEulerXYZ(this, x, y, z);
#else
		throw not_implemented_exception();
#endif
	}

	Matrix33 Matrix33::transpose() {
#if defined(SE_NI_MATRIX33_FNADDR_TRANSPOSE) && SE_NI_MATRIX33_FNADDR_TRANSPOSE > 0
		const auto NI_Matrix33_transpose = reinterpret_cast<Matrix33 * (__thiscall*)(Matrix33*, Matrix33*)>(SE_NI_MATRIX33_FNADDR_TRANSPOSE);

		Matrix33 result;
		NI_Matrix33_transpose(this, &result);
		return result;
#else
		Matrix33 result = *this;
		std::swap(result.m0.y, result.m1.x);
		std::swap(result.m0.z, result.m2.x);
		std::swap(result.m1.z, result.m2.y);
		return result;
#endif
	}

	Matrix33 Matrix33::invert() const {
#if defined(SE_NI_MATRIX33_FNADDR_INVERSE) && SE_NI_MATRIX33_FNADDR_INVERSE > 0
		const auto NI_Matrix33_inverse = reinterpret_cast<Matrix33 * (__thiscall*)(const Matrix33*, Matrix33*)>(SE_NI_MATRIX33_FNADDR_INVERSE);

		Matrix33 result;
		NI_Matrix33_inverse(this, &result);
		return result;
#else
		throw not_implemented_exception();
#endif
	}

	bool Matrix33::invert(Matrix33* out_matrix) const {
#if defined(SE_NI_MATRIX33_FNADDR_INVERSERAW) && SE_NI_MATRIX33_FNADDR_INVERSERAW > 0
		const auto NI_Matrix33_inverseRaw = reinterpret_cast<bool(__thiscall*)(const Matrix33*, Matrix33*)>(SE_NI_MATRIX33_FNADDR_INVERSERAW);

		return NI_Matrix33_inverseRaw(this, out_matrix);
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	std::tuple<Matrix33, bool> Matrix33::invert_lua() const {
		Matrix33 matrix;
		bool valid = invert(&matrix);
		return std::make_tuple(matrix, valid);
	}
#endif

	bool Matrix33::reorthogonalize() {
#if defined(SE_NI_MATRIX33_FNADDR_REORTHOGONALIZE) && SE_NI_MATRIX33_FNADDR_REORTHOGONALIZE > 0
		const auto NI_Matrix33_reorthogonalize = reinterpret_cast<bool(__thiscall*)(Matrix33*)>(SE_NI_MATRIX33_FNADDR_REORTHOGONALIZE);

		return NI_Matrix33_reorthogonalize(this);
#else
		throw not_implemented_exception();
#endif
	}

	bool Matrix33::toEulerXYZ(Vector3* vector) const {
		return toEulerXYZ(&vector->x, &vector->y, &vector->z);
	}

	bool Matrix33::toEulerXYZ(float* x, float* y, float* z) const {
#if defined(SE_NI_MATRIX33_FNADDR_TOEULERXYZ) && SE_NI_MATRIX33_FNADDR_TOEULERXYZ > 0
		const auto NI_Matrix33_toEulerXYZ = reinterpret_cast<bool(__thiscall*)(const Matrix33*, float*, float*, float*)>(SE_NI_MATRIX33_FNADDR_TOEULERXYZ);

		return NI_Matrix33_toEulerXYZ(this, x, y, z);
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	std::tuple<Vector3, bool> Matrix33::toEulerXYZ_lua() const {
		float x, y, z;
		bool isUnique = toEulerXYZ(&x, &y, &z);
		return std::make_tuple(Vector3(x, y, z), isUnique);
	}
#endif

	bool Matrix33::toEulerZYX(Vector3* vector) const {
		return toEulerZYX(&vector->x, &vector->y, &vector->z);
	}

	bool Matrix33::toEulerZYX(float* x, float* y, float* z) const {
		using se::math::M_PI_2f;

		*x = 0;
		*y = asin(m2.x);
		*z = 0;

		if (*y < M_PI_2f) {
			if (*y > -M_PI_2f) {
				*z = -atan2(m1.x, m0.x);
				*x = -atan2(m2.y, m2.z);
				return true;
			}
			else {
				*z = atan2(-m0.y, m0.z);
				return false;
			}
		}
		else {
			*z = -atan2(-m0.y, m0.z);
			return false;
		}
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	std::tuple<Vector3, bool> Matrix33::toEulerZYX_lua() const {
		float x, y, z = 0.0f;
		bool isUnique = toEulerZYX(&x, &y, &z);
		return std::make_tuple(Vector3(x, y, z), isUnique);
	}
#endif

	NI::Quaternion Matrix33::toQuaternion() {
		NI::Quaternion result;
		result.fromRotation(this);
		return result;
	}
}
