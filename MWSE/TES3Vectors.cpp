#include "TES3Vectors.h"

#include "NIColor.h"
#include "NINode.h"
#include "NIQuaternion.h"

#include "MathUtil.h"

namespace TES3 {
	//
	// Vector2
	//
	const Vector2 Vector2::UNIT_X = { 1.0f, 0.0f };
	const Vector2 Vector2::UNIT_NEG_X = { -1.0f, 0.0f };
	const Vector2 Vector2::UNIT_Y = { 0.0f, 1.0f };
	const Vector2 Vector2::UNIT_NEG_Y = { 0.0f, -1.0f };
	const Vector2 Vector2::ONES = { 1.0f, 1.0f };
	const Vector2 Vector2::ZEROES = { 0.0f, 0.0f };

	Vector2::Vector2() :
		x(0),
		y(0)
	{

	}

	Vector2::Vector2(float _x, float _y) :
		x(_x),
		y(_y)
	{

	}

	Vector2::Vector2(sol::table table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
	}

	Vector2& Vector2::operator=(const sol::table table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
		return *this;
	}

	bool Vector2::operator==(const Vector2& vec3) const {
		return x == vec3.x && y == vec3.y;
	}

	bool Vector2::operator!=(const Vector2& vec3) const {
		return x != vec3.x || y != vec3.y;
	}

	Vector2 Vector2::operator+(const Vector2& vec3) const {
		return Vector2(x + vec3.x, y + vec3.y);
	}

	Vector2 Vector2::operator-(const Vector2& vec3) const {
		return Vector2(x - vec3.x, y - vec3.y);
	}

	Vector2 Vector2::operator*(const Vector2& vec3) const {
		return Vector2(x * vec3.x, y * vec3.y);
	}

	Vector2 Vector2::operator*(const float scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 Vector2::operator/(const float scalar) const {
		return { x / scalar, y / scalar };
	}

	std::ostream& operator<<(std::ostream& str, const Vector2& vector) {
		str << "(" << vector.x << "," << vector.y << ")";
		return str;
	}

	float Vector2::distance(const Vector2* vec2) const {
		float dx = x - vec2->x;
		float dy = y - vec2->y;
		return sqrt(dx * dx + dy * dy);
	}

	
	float Vector2::distanceChebyshev(const Vector2* vec2) const {
		float dx = fabs(x - vec2->x);
		float dy = fabs(y - vec2->y);
		return std::max(dx, dy);
	}

	float Vector2::distanceManhattan(const Vector2* vec2) const {
		float dx = fabs(x - vec2->x);
		float dy = fabs(y - vec2->y);
		return dx + dy;
	}

	std::string Vector2::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Vector2::toJson() const {
		std::ostringstream ss;
		ss << "{\"x\":" << x << ",\"y\":" << y << "}";
		return std::move(ss.str());
	}

	Vector2 Vector2::copy() const {
		return *this;
	}

	Vector2 Vector2::min(const Vector2& other) const {
		return Vector2(std::min(x, other.x), std::min(y, other.y));
	}

	Vector2 Vector2::max(const Vector2& other) const {
		return Vector2(std::max(x, other.x), std::max(y, other.y));
	}

	float Vector2::length() const {
		return sqrt(x * x + y * y);
	}

	bool Vector2::normalize() {
		float len = length();
		if (len > mwse::math::M_NORMALIZE_EPSILON) {
			x = x / len;
			y = y / len;
			return true;
		}
		x = 0;
		y = 0;
		return false;
	}

	Vector2 Vector2::normalized() const {
		auto copy = Vector2(x, y);
		copy.normalize();
		return copy;
	}

	//
	// Vector3
	//

	const Vector3 Vector3::UNIT_X = { 1.0f, 0.0f, 0.0f };
	const Vector3 Vector3::UNIT_NEG_X = { -1.0f, 0.0f, 0.0f };
	const Vector3 Vector3::UNIT_Y = { 0.0f, 1.0f, 0.0f };
	const Vector3 Vector3::UNIT_NEG_Y = { 0.0f, -1.0f, 0.0f };
	const Vector3 Vector3::UNIT_Z = { 0.0f, 0.0f, 1.0f };
	const Vector3 Vector3::UNIT_NEG_Z = { 0.0f, 0.0f, -1.0f };
	const Vector3 Vector3::ONES = { 1.0f, 1.0f, 1.0f };
	const Vector3 Vector3::ZEROES = { 0.0f, 0.0f, 0.0f };

	Vector3::Vector3() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{

	}

	Vector3::Vector3(float _x, float _y, float _z) :
		x(_x),
		y(_y),
		z(_z)
	{

	}

	Vector3::Vector3(NI::Color& color) {
		x = color.r;
		y = color.g;
		z = color.b;
	}

	Vector3::Vector3(sol::table table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
		z = table.get_or("z", table.get_or(3, 0.0f));
	}

	Vector3::Vector3(sol::object object) {
		if (object.is<NI::Color>()) {
			*this = NI::Color(object.as<NI::Color>());
		}
		else if (object.is<sol::table>()) {
			*this = NI::Color(object.as<sol::table>());
		}
		throw std::invalid_argument("Could not convert lua object to TES3Vector3.");
	}

	Vector3& Vector3::operator=(const NI::Color& vector) {
		x = vector.r;
		y = vector.g;
		z = vector.b;
		return *this;
	}

	Vector3& Vector3::operator=(const sol::table table) {
		x = table.get_or("x", table.get_or(1, 0.0f));
		y = table.get_or("y", table.get_or(2, 0.0f));
		z = table.get_or("z", table.get_or(3, 0.0f));
		return *this;
	}

	Vector3& Vector3::operator=(const sol::object object) {
		if (object.is<NI::Color>()) {
			*this = object.as<NI::Color>();
		}
		else if (object.is<sol::table>()) {
			*this = object.as<sol::table>();
		}
		else {
			throw std::invalid_argument("Could not convert lua object to NiColor.");
		}
		return *this;
	}

	bool Vector3::operator==(const Vector3& vec3) const {
		return x == vec3.x && y == vec3.y && z == vec3.z;
	}

	bool Vector3::operator!=(const Vector3& vec3) const {
		return x != vec3.x || y != vec3.y || z != vec3.z;
	}

	Vector3 Vector3::operator+(const Vector3& vec3) const {
		return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
	}

	Vector3 Vector3::operator+(const float value) const {
		return Vector3(x + value, y + value, z + value);
	}

	Vector3 Vector3::operator-(const Vector3& vec3) const {
		return Vector3(x - vec3.x, y - vec3.y, z - vec3.z);
	}

	Vector3 Vector3::operator-(const float value) const {
		return Vector3(x - value, y - value, z - value);
	}

	Vector3 Vector3::operator-() const {
		return Vector3(-x, -y, -z);
	}

	Vector3 Vector3::operator*(const Vector3& vec3) const {
		return Vector3(x * vec3.x, y * vec3.y, z * vec3.z);
	}

	Vector3 Vector3::operator*(const float scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	Vector3 Vector3::operator/(const float scalar) const {
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	std::ostream& operator<<(std::ostream& str, const Vector3& vector) {
		str << "(" << vector.x << "," << vector.y << "," << vector.z << ")";
		return str;
	}

	std::string Vector3::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Vector3::toJson() const {
		std::ostringstream ss;
		ss << "{\"x\":" << x << ",\"y\":" << y << ",\"z\":" << z << "}";
		return std::move(ss.str());
	}

	Vector3 Vector3::copy() const {
		return *this;
	}

	Vector3 Vector3::min(const Vector3& other) const {
		return Vector3(std::min(x, other.x), std::min(y, other.y), std::min(z, other.z));
	}

	Vector3 Vector3::max(const Vector3& other) const {
		return Vector3(std::max(x, other.x), std::max(y, other.y), std::max(z, other.z));
	}

	NI::Color Vector3::toNiColor() const {
		return NI::Color(x, y, z);
	}

	Vector3 Vector3::crossProduct(const Vector3* vec3) const {
		return Vector3(y * vec3->z - z * vec3->y, z * vec3->x - vec3->z * x, x * vec3->y - y * vec3->x);
	}

	float Vector3::dotProduct(const Vector3* vec3) const {
		return vec3->z * z + vec3->y * y + vec3->x * x;
	}

	Matrix33 Vector3::outerProduct(const Vector3* vec3) const {
		return Matrix33(
			(x * vec3->x), (y * vec3->x), (z * vec3->x),
			(x * vec3->y), (y * vec3->y), (z * vec3->y),
			(x * vec3->z), (y * vec3->z), (z * vec3->z)
		);
	}

	Vector3 Vector3::lerp(const Vector3& to, float transition) const {
		auto transA = 1.0f - transition;
		return Vector3(x * transA + to.x * transition, y * transA + to.y * transition, z * transA + to.z * transition);
	}

	float Vector3::heightDifference(const Vector3* vec3) const {
		return fabs(z - vec3->z);
	}

	float Vector3::distance(const Vector3* vec3) const {
		float dx = x - vec3->x;
		float dy = y - vec3->y;
		float dz = z - vec3->z;
		return sqrt(dz * dz + dx * dx + dy * dy);
	}

	float Vector3::distanceChebyshev(const Vector3* vec3) const {
		float dx = fabs(x - vec3->x);
		float dy = fabs(y - vec3->y);
		float dz = fabs(z - vec3->z);
		return std::max(std::max(dx, dy), dz);
	}

	float Vector3::distanceManhattan(const Vector3* vec3) const {
		float dx = fabs(x - vec3->x);
		float dy = fabs(y - vec3->y);
		float dz = fabs(z - vec3->z);
		return dx + dy + dz;
	}

	float Vector3::distanceXY(const Vector3* vec3) const {
		float dx = x - vec3->x;
		float dy = y - vec3->y;
		return sqrt(dx * dx + dy * dy);
	}

	float Vector3::angle(const Vector3* v) const {
		// Numerically stable version, from Kahan.
		// Avoids an issue where the dot product is marginally out of domain for acos.
		auto a_unit = normalized();
		auto b_unit = v->normalized();
		return 2.0f * atan2f((a_unit - b_unit).length(), (a_unit + b_unit).length());
	}

	float Vector3::length() const {
		return sqrt(x * x + y * y + z * z);
	}

	void Vector3::negate() {
		x = -x;
		y = -y;
		z = -z;
	}

	bool Vector3::normalize() {
		const auto len = length();
		if (len > mwse::math::M_NORMALIZE_EPSILON) {
			x = x / len;
			y = y / len;
			z = z / len;
			return true;
		}
		else {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			return false;
		}
	}

	Vector3 Vector3::normalized() const {
		auto copy = Vector3(x, y, z);
		copy.normalize();
		return copy;
	}

	Vector3 Vector3::interpolate(const Vector3& targetPoint, const float distance) const {
		auto line = targetPoint - *this;
		if (line.normalize()) {
			return *this + (line * distance);
		}
		return Vector3();
	}

	bool Vector3::canConvertFrom(sol::table& table) {
		sol::optional<float> x = table["x"];
		if (!x) x = table[1];
		sol::optional<float> y = table["y"];
		if (!y) y = table[2];
		sol::optional<float> z = table["z"];
		if (!z) z = table[3];
		return x && y && z;
	}

	//
	// Vector4
	//

	Vector4::Vector4() :
		x(0.0f),
		y(0.0f),
		z(0.0f),
		w(0.0f)
	{

	}

	Vector4::Vector4(float _x, float _y, float _z, float _w) :
		x(_x),
		y(_y),
		z(_z),
		w(_w)
	{

	}

	bool Vector4::operator==(const Vector4& other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool Vector4::operator!=(const Vector4& other) const {
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}

	Vector4 Vector4::operator+(const Vector4& other) const {
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vector4 Vector4::operator-(const Vector4& other) const {
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	Vector4 Vector4::operator*(const Vector4& other) const {
		return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	Vector4 Vector4::operator*(const float scalar) const {
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	Vector4 Vector4::operator/(const float scalar) const {
		return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	std::ostream& operator<<(std::ostream& str, const Vector4& vector) {
		str << "(" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << ")";
		return str;
	}

	std::string Vector4::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Vector4::toJson() const {
		std::ostringstream ss;
		ss << "{\"x\":" << x << ",\"y\":" << y << ",\"z\":" << z << ",\"w\":" << w << "}";
		return std::move(ss.str());
	}

	Vector4 Vector4::copy() const {
		return *this;
	}

	Vector4 Vector4::min(const Vector4& other) const {
		return Vector4(std::min(x, other.x), std::min(y, other.y), std::min(z, other.z), std::min(w, other.w));
	}

	Vector4 Vector4::max(const Vector4& other) const {
		return Vector4(std::max(x, other.x), std::max(y, other.y), std::max(z, other.z), std::max(w, other.w));
	}

	float Vector4::distance(const Vector4* vec4) const {
		float dx = x - vec4->x;
		float dy = y - vec4->y;
		float dz = z - vec4->z;
		float dw = w - vec4->w;
		return sqrt(dz * dz + dw * dw + dx * dx + dy * dy);
	}

	float Vector4::distanceChebyshev(const Vector4* vec4) const {
		float dx = fabs(x - vec4->x);
		float dy = fabs(y - vec4->y);
		float dz = fabs(z - vec4->z);
		float dw = fabs(w - vec4->w);
		return std::max(std::max(dx, dy), std::max(dz, dw));
	}

	float Vector4::distanceManhattan(const Vector4* vec4) const {
		float dx = fabs(x - vec4->x);
		float dy = fabs(y - vec4->y);
		float dz = fabs(z - vec4->z);
		float dw = fabs(w - vec4->w);
		return dx + dy + dz + dw;
	}

	float Vector4::length() const {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	//
	// Matrix33
	//

	const auto TES3_Matrix33_testEqual = reinterpret_cast<bool(__thiscall*)(Matrix33*, const Matrix33*)>(0x6E7ED0);
	const auto TES3_Matrix33_addMatrix = reinterpret_cast<Matrix33 * (__thiscall*)(Matrix33*, Matrix33*, const Matrix33*)>(0x6E7F60);
	const auto TES3_Matrix33_subtractMatrix = reinterpret_cast<Matrix33 * (__thiscall*)(Matrix33*, Matrix33*, const Matrix33*)>(0x6E8000);
	const auto TES3_Matrix33_multiplyMatrix = reinterpret_cast<Matrix33 * (__thiscall*)(Matrix33*, Matrix33*, const Matrix33*)>(0x6E80A0);
	const auto TES3_Matrix33_multiplyVector = reinterpret_cast<Vector3 * (__thiscall*)(const Matrix33*, Vector3*, const Vector3*)>(0x6E8230);
	const auto TES3_Matrix33_multiplyScalar = reinterpret_cast<Matrix33 * (__thiscall*)(Matrix33*, Matrix33*, float)>(0x6E81B0);

	const auto TES3_Matrix33_toIdentity = reinterpret_cast<void(__thiscall*)(Matrix33*)>(0x6E7CF0);
	const auto TES3_Matrix33_toRotationX = reinterpret_cast<void(__thiscall*)(Matrix33*, float)>(0x6E7D20);
	const auto TES3_Matrix33_toRotationY = reinterpret_cast<void(__thiscall*)(Matrix33*, float)>(0x6E7D60);
	const auto TES3_Matrix33_toRotationZ = reinterpret_cast<void(__thiscall*)(Matrix33*, float)>(0x6E7DA0);
	const auto TES3_Matrix33_toRotationXYZ = reinterpret_cast<void(__thiscall*)(Matrix33*, float, float, float, float)> (0x6E7DE0);

	const auto TES3_Matrix33_fromEulerXYZ = reinterpret_cast<void(__thiscall*)(Matrix33*, float, float, float)> (0x6E8D60);
	const auto TES3_Matrix33_toEulerXYZ = reinterpret_cast<bool(__thiscall*)(const Matrix33*, float*, float*, float*)> (0x6E8C50);

	const auto TES3_Matrix33_transpose = reinterpret_cast<Matrix33 * (__thiscall*)(Matrix33*, Matrix33*)> (0x6E8420);
	const auto TES3_Matrix33_transposeMult = reinterpret_cast<Vector3 * (__cdecl*)(Vector3*, const Vector3*, const Matrix33*)> (0x6E8290);
	const auto TES3_Matrix33_inverseRaw = reinterpret_cast<bool(__thiscall*)(const Matrix33*, Matrix33*)> (0x6E82F0);
	const auto TES3_Matrix33_inverse = reinterpret_cast<Matrix33 * (__thiscall*)(const Matrix33*, Matrix33*)> (0x6E83E0);

	const auto TES3_Matrix33_reorthogonalize = reinterpret_cast<bool(__thiscall*)(Matrix33*)> (0x6E84A0);

	const auto NI_Quaternion_FromRotation = reinterpret_cast<TES3::Matrix33 * (__thiscall*)(const NI::Quaternion*, TES3::Matrix33*)>(0x6FBEF0);

	const Matrix33 Matrix33::IDENTITY = { 
		1.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 1.0f 
	};

	Matrix33::Matrix33() : m0(), m1(), m2() {

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

	bool Matrix33::operator==(const Matrix33& matrix) {
		return TES3_Matrix33_testEqual(this, &matrix);
	}

	bool Matrix33::operator!=(const Matrix33& matrix) {
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

	Matrix33 Matrix33::operator+(const Matrix33& matrix) {
		Matrix33 result;
		TES3_Matrix33_addMatrix(this, &result, &matrix);
		return result;
	}

	Matrix33 Matrix33::operator-(const Matrix33& matrix) {
		Matrix33 result;
		TES3_Matrix33_subtractMatrix(this, &result, &matrix);
		return result;
	}

	Matrix33 Matrix33::operator*(const Matrix33& matrix) {
		Matrix33 result;
		TES3_Matrix33_multiplyMatrix(this, &result, &matrix);
		return result;
	}

	Vector3 Matrix33::operator*(const Vector3& vector) const {
		Vector3 result;
		TES3_Matrix33_multiplyVector(this, &result, &vector);
		return result;
	}

	Matrix33 Matrix33::operator*(float scalar) {
		Matrix33 result;
		TES3_Matrix33_multiplyScalar(this, &result, scalar);
		return result;
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
		TES3_Matrix33_toIdentity(this);
	}

	void Matrix33::toRotationX(float value) {
		TES3_Matrix33_toRotationX(this, value);
	}

	void Matrix33::toRotationY(float value) {
		TES3_Matrix33_toRotationY(this, value);
	}

	void Matrix33::toRotationZ(float value) {
		TES3_Matrix33_toRotationZ(this, value);
	}

	void Matrix33::toRotation(float angle, float x, float y, float z) {
		TES3_Matrix33_toRotationXYZ(this, angle, x, y, z);
	}

	void Matrix33::fromEulerXYZ(float x, float y, float z) {
		TES3_Matrix33_fromEulerXYZ(this, x, y, z);
	}

	Matrix33 Matrix33::transpose() {
		Matrix33 result;
		TES3_Matrix33_transpose(this, &result);
		return result;
	}

	Vector3 Matrix33::transposeMult(const Vector3& vector) const {
		Vector3 result;
		TES3_Matrix33_transposeMult(&result, &vector, this);
		return result;
	}

	Matrix33 Matrix33::invert() const {
		Matrix33 result;
		TES3_Matrix33_inverse(this, &result);
		return result;
	}

	bool Matrix33::invert(Matrix33* out_matrix) const {
		return TES3_Matrix33_inverseRaw(this, out_matrix);
	}

	std::tuple<Matrix33, bool> Matrix33::invert_lua() const {
		Matrix33 matrix;
		bool valid = invert(&matrix);
		return std::make_tuple(matrix, valid);
	}

	bool Matrix33::reorthogonalize() {
		return TES3_Matrix33_reorthogonalize(this);
	}

	bool Matrix33::toEulerXYZ(Vector3* vector) const {
		return TES3_Matrix33_toEulerXYZ(this, &vector->x, &vector->y, &vector->z);
	}

	bool Matrix33::toEulerXYZ(float * x, float * y, float * z) const {
		return TES3_Matrix33_toEulerXYZ(this, x, y, z);
	}

	std::tuple<Vector3, bool> Matrix33::toEulerXYZ_lua() const {
		float x, y, z;
		bool isUnique = toEulerXYZ(&x, &y, &z);
		return std::make_tuple(Vector3(x, y, z), isUnique);
	}

	bool Matrix33::toEulerZYX(Vector3* vector) const {
		return toEulerZYX(&vector->x, &vector->y, &vector->z);
	}

	bool Matrix33::toEulerZYX(float* x, float* y, float* z) const {
		*x = 0; 
		*y = asin(m2.x);
		*z = 0;

		if (*y < mwse::math::M_PI_2) {
			if (*y > -mwse::math::M_PI_2) {
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

	std::tuple<Vector3, bool> Matrix33::toEulerZYX_lua() const {
		float x, y, z = 0.0f;
		bool isUnique = toEulerZYX(&x, &y, &z);
		return std::make_tuple(Vector3(x, y, z), isUnique);
	}

	void Matrix33::fromQuaternion(const NI::Quaternion* q) {
		NI_Quaternion_FromRotation(q, this);
	}

	NI::Quaternion Matrix33::toQuaternion() const {
		NI::Quaternion result;
		result.fromRotation(this);
		return result;
	}

	Vector3 Matrix33::getForwardVector() {
		return Vector3(m0.y, m1.y, m2.y);
	}

	Vector3 Matrix33::getRightVector() {
		return Vector3(m0.x, m1.x, m2.x);
	}

	Vector3 Matrix33::getUpVector() {
		return Vector3(m0.z, m1.z, m2.z);
	}

	void Matrix33::lookAt(const Vector3& direction, const Vector3& worldUp) {
		const auto forward = direction.normalized();
		auto left = worldUp.crossProduct(&forward);
		if (left.dotProduct(&left) < mwse::math::M_NORMALIZE_EPSILON) {
			left = forward.crossProduct(&Vector3::UNIT_NEG_Y);
		}
		left.normalize();
		const auto up = forward.crossProduct(&left);
		m0.x = -left.x;
		m1.x = -left.y;
		m2.x = -left.z;
		m0.y = forward.x;
		m1.y = forward.y;
		m2.y = forward.z;
		m0.z = up.x;
		m1.z = up.y;
		m2.z = up.z;
	}

	//
	// Matrix44
	//

	const Matrix44 Matrix44::IDENTITY = { 
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f 
	};

	Matrix44::Matrix44() :
		m0(),
		m1(),
		m2(),
		m3()
	{

	}

	Matrix44::Matrix44(const Vector4& in_m0, const Vector4& in_m1, const Vector4& in_m2, const Vector4& in_m3) :
		m0(in_m0),
		m1(in_m1),
		m2(in_m2),
		m3(in_m3)
	{

	}

	Matrix44::Matrix44(float m0w, float m0x, float m0y, float m0z, float m1w, float m1x, float m1y, float m1z, float m2w, float m2x, float m2y, float m2z, float m3w, float m3x, float m3y, float m3z) :
		m0(m0w, m0x, m0y, m0z),
		m1(m1w, m1x, m1y, m1z),
		m2(m2w, m2x, m2y, m2z),
		m3(m3w, m3x, m3y, m3z)
	{

	}

	bool Matrix44::operator==(const Matrix44& matrix) {
		return m0 == matrix.m0 && m1 == matrix.m1 && m2 == matrix.m2 && m3 == matrix.m3;
	}

	bool Matrix44::operator!=(const Matrix44& matrix) {
		return m0 != matrix.m0 || m1 != matrix.m1 || m2 != matrix.m2 || m3 != matrix.m3;
	}

	Matrix44 Matrix44::operator+(const Matrix44& matrix) {
		return Matrix44(m0 + matrix.m0, m1 + matrix.m1, m2 + matrix.m2, m3 + matrix.m3);
	}

	Matrix44 Matrix44::operator-(const Matrix44& matrix) {
		return Matrix44(m0 - matrix.m0, m1 - matrix.m1, m2 - matrix.m2, m3 - matrix.m3);
	}

	Matrix44 Matrix44::operator*(const Matrix44& matrix) {
		return Matrix44(
			m0.w * matrix.m0.w + m0.x * matrix.m1.w + m0.y * matrix.m2.w + m0.z * matrix.m3.w,
			m0.w * matrix.m0.x + m0.x * matrix.m1.x + m0.y * matrix.m2.x + m0.z * matrix.m3.x,
			m0.w * matrix.m0.y + m0.x * matrix.m1.y + m0.y * matrix.m2.y + m0.z * matrix.m3.y,
			m0.w * matrix.m0.z + m0.x * matrix.m1.z + m0.y * matrix.m2.z + m0.z * matrix.m3.z,

			m1.w * matrix.m0.w + m1.x * matrix.m1.w + m1.y * matrix.m2.w + m1.z * matrix.m3.w,
			m1.w * matrix.m0.x + m1.x * matrix.m1.x + m1.y * matrix.m2.x + m1.z * matrix.m3.x,
			m1.w * matrix.m0.y + m1.x * matrix.m1.y + m1.y * matrix.m2.y + m1.z * matrix.m3.y,
			m1.w * matrix.m0.z + m1.x * matrix.m1.z + m1.y * matrix.m2.z + m1.z * matrix.m3.z,

			m2.w * matrix.m0.w + m2.x * matrix.m1.w + m2.y * matrix.m2.w + m2.z * matrix.m3.w,
			m2.w * matrix.m0.x + m2.x * matrix.m1.x + m2.y * matrix.m2.x + m2.z * matrix.m3.x,
			m2.w * matrix.m0.y + m2.x * matrix.m1.y + m2.y * matrix.m2.y + m2.z * matrix.m3.y,
			m2.w * matrix.m0.z + m2.x * matrix.m1.z + m2.y * matrix.m2.z + m2.z * matrix.m3.z,

			m3.w * matrix.m0.w + m3.x * matrix.m1.w + m3.y * matrix.m2.w + m3.z * matrix.m3.w,
			m3.w * matrix.m0.x + m3.x * matrix.m1.x + m3.y * matrix.m2.x + m3.z * matrix.m3.x,
			m3.w * matrix.m0.y + m3.x * matrix.m1.y + m3.y * matrix.m2.y + m3.z * matrix.m3.y,
			m3.w * matrix.m0.z + m3.x * matrix.m1.z + m3.y * matrix.m2.z + m3.z * matrix.m3.z
		);
	}

	Matrix44 Matrix44::operator*(float scalar) {
		return Matrix44(m0 * scalar, m1 * scalar, m2 * scalar, m3 * scalar);
	}

	std::ostream& operator<<(std::ostream& str, const Matrix44& matrix) {
		str << "[" << matrix.m0 << "," << matrix.m1 << "," << matrix.m2 << "," << matrix.m3 << "]";
		return str;
	}

	std::string Matrix44::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string Matrix44::toJson() const {
		std::ostringstream ss;
		ss << "["
			<< m0.toJson() << ","
			<< m1.toJson() << ","
			<< m2.toJson() << ","
			<< m3.toJson()
			<< "]";
		return std::move(ss.str());
	}

	Matrix44 Matrix44::copy() const {
		return *this;
	}

	void Matrix44::toZero() {
		m0.w = 0.0f;
		m0.x = 0.0f;
		m0.y = 0.0f;
		m0.z = 0.0f;
		m1.w = 0.0f;
		m1.x = 0.0f;
		m1.y = 0.0f;
		m1.z = 0.0f;
		m2.w = 0.0f;
		m2.x = 0.0f;
		m2.y = 0.0f;
		m2.z = 0.0f;
		m3.w = 0.0f;
		m3.x = 0.0f;
		m3.y = 0.0f;
		m3.z = 0.0f;
	}

	//
	// BoundingBox
	//

	BoundingBox::BoundingBox() :
		minimum(),
		maximum()
	{

	}

	BoundingBox::BoundingBox(const Vector3& min, const Vector3& max) :
		minimum(min),
		maximum(max)
	{

	}

	BoundingBox::BoundingBox(const BoundingBox& bbox) :
		minimum(bbox.minimum),
		maximum(bbox.maximum)
	{

	}

	BoundingBox::BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) :
		minimum(minX, minY, minZ),
		maximum(maxX, maxY, maxZ)
	{

	}

	bool BoundingBox::operator==(const BoundingBox& other) const {
		return minimum == other.minimum && maximum == other.maximum;
	}

	bool BoundingBox::operator!=(const BoundingBox& other) const {
		return minimum != other.minimum || maximum != other.maximum;
	}

	std::ostream& operator<<(std::ostream& str, const BoundingBox& other) {
		str << "(" << other.minimum << "," << other.maximum << ")";
		return str;
	}

	std::string BoundingBox::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string BoundingBox::toJson() const {
		std::ostringstream ss;
		ss << "{"
			<< "\"min\":" << minimum.toJson() << ","
			<< "\"max\":" << maximum.toJson()
			<< "}";
		return std::move(ss.str());
	}

	BoundingBox BoundingBox::copy() const {
		return *this;
	}

	std::array<Vector3, 8> BoundingBox::vertices() const {
		return std::array {
			Vector3(minimum.x, minimum.y, minimum.z),
			Vector3(minimum.x, minimum.y, maximum.z),
			Vector3(minimum.x, maximum.y, minimum.z),
			Vector3(minimum.x, maximum.y, maximum.z),
			Vector3(maximum.x, maximum.y, minimum.z),
			Vector3(maximum.x, minimum.y, maximum.z),
			Vector3(maximum.x, minimum.y, minimum.z),
			Vector3(maximum.x, maximum.y, maximum.z),
		};
	}

	void BoundingBox::clampPoint(Vector3& point, const Vector3& origin) const {
		const auto min = minimum + origin;
		const auto max = maximum + origin;
		//const auto min = minimum;
		//const auto max = maximum;
		point.x = std::clamp(point.x, min.x, max.x);
		point.y = std::clamp(point.y, min.y, max.y);
		point.z = std::clamp(point.z, min.z, max.z);
	}

	//
	// Transform
	//

	Transform::Transform() {
		this->toIdentity();
	}

	Transform::Transform(const Matrix33& rotation, const Vector3& translation, const float scale) :
		rotation(rotation),
		translation(translation),
		scale(scale)
	{
	}

	Transform Transform::operator*(const Transform& transform) {
		return {
			rotation * transform.rotation,
			rotation * transform.translation * scale + translation,
			scale * transform.scale
		};
	}

	Vector3 Transform::operator*(const Vector3& vector) {
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
		translation.x = 0.0f;
		translation.y = 0.0f;
		translation.z = 0.0f;
		scale = 1.0f;
	}
}

