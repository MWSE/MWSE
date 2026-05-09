#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIColor.h"
#include "NIMatrix33.h"
#include "NIMatrix44.h"
#include "NIRange.h"
#include "NIQuaternion.h"
#include "NIVector2.h"
#include "NIVector3.h"
#include "NIVector4.h"

namespace mwse::lua {
	void bindTES3Vectors() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::Range<int>.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Range<int>>("tes3rangeInt");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property bindings.
			usertypeDefinition["min"] = &NI::Range<int>::min;
			usertypeDefinition["max"] = &NI::Range<int>::max;
		}

		// Binding for NI::Vector2.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Vector2>("tes3vector2");
			usertypeDefinition["new"] = sol::constructors<NI::Vector2(), NI::Vector2(float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &NI::Vector2::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &NI::Vector2::operator-;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Vector2(const NI::Vector2&) const>(&NI::Vector2::operator*),
				sol::resolve<NI::Vector2(const float) const>(&NI::Vector2::operator*)
			);
			usertypeDefinition[sol::meta_function::division] = &NI::Vector2::operator/;
			usertypeDefinition[sol::meta_function::length] = &NI::Vector2::length;
			usertypeDefinition[sol::meta_function::to_string] = &NI::Vector2::toString;

			// Allow objects to be serialized to json using their ID.
			usertypeDefinition["__tojson"] = &NI::Vector2::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &NI::Vector2::x;
			usertypeDefinition["y"] = &NI::Vector2::y;

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::Vector2::copy;
			usertypeDefinition["distance"] = &NI::Vector2::distance;
			usertypeDefinition["distanceChebyshev"] = &NI::Vector2::distanceChebyshev;
			usertypeDefinition["distanceManhattan"] = &NI::Vector2::distanceManhattan;
			usertypeDefinition["length"] = &NI::Vector2::length;
			usertypeDefinition["normalize"] = &NI::Vector2::normalize;
			usertypeDefinition["normalized"] = &NI::Vector2::normalized;
			usertypeDefinition["min"] = &NI::Vector2::min;
			usertypeDefinition["max"] = &NI::Vector2::max;

			// Alternate constructors.
			usertypeDefinition["unitX"] = sol_copy_wrapper(NI::Vector2::UNIT_X);
			usertypeDefinition["unitY"] = sol_copy_wrapper(NI::Vector2::UNIT_Y);
			usertypeDefinition["ones"] = sol_copy_wrapper(NI::Vector2::ONES);
			usertypeDefinition["zeroes"] = sol_copy_wrapper(NI::Vector2::ZEROES);
		}

		// Binding for NI::Vector3.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Vector3>("tes3vector3");
			usertypeDefinition["new"] = sol::constructors<NI::Vector3(), NI::Vector3(float, float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = sol::overload(
				sol::resolve<NI::Vector3(const NI::Vector3&) const>(&NI::Vector3::operator+),
				sol::resolve<NI::Vector3(const float) const>(&NI::Vector3::operator+)
			);
			usertypeDefinition[sol::meta_function::subtraction] = sol::overload(
				sol::resolve<NI::Vector3(const NI::Vector3&) const>(&NI::Vector3::operator-),
				sol::resolve<NI::Vector3(const float) const>(&NI::Vector3::operator-)
			);
			usertypeDefinition[sol::meta_function::unary_minus] = sol::resolve<NI::Vector3() const>(&NI::Vector3::operator-);;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Vector3(const NI::Vector3&) const>(&NI::Vector3::operator*),
				sol::resolve<NI::Vector3(const float) const>(&NI::Vector3::operator*)
			);
			usertypeDefinition[sol::meta_function::division] = &NI::Vector3::operator/;
			usertypeDefinition[sol::meta_function::length] = &NI::Vector3::length;
			usertypeDefinition[sol::meta_function::to_string] = &NI::Vector3::toString;

			// Allow objects to be serialized to json using their ID.
			usertypeDefinition["__tojson"] = &NI::Vector3::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &NI::Vector3::x;
			usertypeDefinition["y"] = &NI::Vector3::y;
			usertypeDefinition["z"] = &NI::Vector3::z;

			// These can also be used for RGB.
			usertypeDefinition["r"] = &NI::Vector3::x;
			usertypeDefinition["g"] = &NI::Vector3::y;
			usertypeDefinition["b"] = &NI::Vector3::z;

			// Basic function binding.
			usertypeDefinition["angle"] = &NI::Vector3::angle;
			usertypeDefinition["copy"] = &NI::Vector3::copy;
			usertypeDefinition["cross"] = &NI::Vector3::crossProduct;
			usertypeDefinition["distance"] = &NI::Vector3::distance;
			usertypeDefinition["distanceChebyshev"] = &NI::Vector3::distanceChebyshev;
			usertypeDefinition["distanceManhattan"] = &NI::Vector3::distanceManhattan;
			usertypeDefinition["distanceXY"] = &NI::Vector3::distanceXY;
			usertypeDefinition["dot"] = &NI::Vector3::dotProduct;
			usertypeDefinition["outerProduct"] = &NI::Vector3::outerProduct;
			usertypeDefinition["heightDifference"] = &NI::Vector3::heightDifference;
			usertypeDefinition["length"] = &NI::Vector3::length;
			usertypeDefinition["lerp"] = &NI::Vector3::lerp;
			usertypeDefinition["negate"] = &NI::Vector3::negate;
			usertypeDefinition["normalize"] = &NI::Vector3::normalize;
			usertypeDefinition["normalized"] = &NI::Vector3::normalized;
			usertypeDefinition["interpolate"] = &NI::Vector3::interpolate;
			usertypeDefinition["min"] = &NI::Vector3::min;
			usertypeDefinition["max"] = &NI::Vector3::max;

			// Conversion to NI::Color.
			usertypeDefinition["toColor"] = &NI::Vector3::toNiColor;

			// Alternate constructors.
			usertypeDefinition["unitX"] = sol_copy_wrapper(NI::Vector3::UNIT_X);
			usertypeDefinition["unitY"] = sol_copy_wrapper(NI::Vector3::UNIT_Y);
			usertypeDefinition["unitZ"] = sol_copy_wrapper(NI::Vector3::UNIT_Z);
			usertypeDefinition["ones"] = sol_copy_wrapper(NI::Vector3::ONES);
			usertypeDefinition["zeroes"] = sol_copy_wrapper(NI::Vector3::ZEROES);
		}

		// Binding for NI::Vector4.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Vector4>("tes3vector4");
			usertypeDefinition["new"] = sol::constructors<NI::Vector4(), NI::Vector4(float, float, float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &NI::Vector4::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &NI::Vector4::operator-;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Vector4(const NI::Vector4&) const>(&NI::Vector4::operator*),
				sol::resolve<NI::Vector4(const float) const>(&NI::Vector4::operator*)
			);
			usertypeDefinition[sol::meta_function::division] = &NI::Vector4::operator/;
			usertypeDefinition[sol::meta_function::length] = &NI::Vector4::length;
			usertypeDefinition[sol::meta_function::to_string] = &NI::Vector4::toString;
			usertypeDefinition["__tojson"] = &NI::Vector4::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &NI::Vector4::x;
			usertypeDefinition["y"] = &NI::Vector4::y;
			usertypeDefinition["z"] = &NI::Vector4::z;
			usertypeDefinition["w"] = &NI::Vector4::w;

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::Vector4::copy;
			usertypeDefinition["distance"] = &NI::Vector4::distance;
			usertypeDefinition["distanceChebyshev"] = &NI::Vector4::distanceChebyshev;
			usertypeDefinition["distanceManhattan"] = &NI::Vector4::distanceManhattan;
			usertypeDefinition["length"] = &NI::Vector4::length;
			usertypeDefinition["min"] = &NI::Vector4::min;
			usertypeDefinition["max"] = &NI::Vector4::max;
		}

		// Binding for NI::BoundingBox.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::BoundingBox>("tes3boundingBox");
			usertypeDefinition["new"] = sol::no_constructor;

			// Operator overloading.
			usertypeDefinition[sol::meta_function::to_string] = &NI::BoundingBox::toString;

			// Allow objects to be serialized to json using their ID.
			usertypeDefinition["__tojson"] = &NI::BoundingBox::toJson;

			// Basic property bindings.
			usertypeDefinition["max"] = &NI::BoundingBox::maximum;
			usertypeDefinition["min"] = &NI::BoundingBox::minimum;

			// Basic function binding.
			usertypeDefinition["clampPoint"] = &NI::BoundingBox::clampPoint;
			usertypeDefinition["copy"] = &NI::BoundingBox::copy;
			usertypeDefinition["hasUninitializedData"] = &NI::BoundingBox::hasUninitializedData;
			usertypeDefinition["initialize"] = &NI::BoundingBox::initialize;
			usertypeDefinition["vertices"] = &NI::BoundingBox::vertices;
		}

		// Binding for NI::Matrix33.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Matrix33>("tes3matrix33");
			usertypeDefinition["new"] = sol::constructors<
				NI::Matrix33(),
				NI::Matrix33(NI::Vector3*, NI::Vector3*, NI::Vector3*),
				NI::Matrix33(float, float, float, float, float, float, float, float, float)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &NI::Matrix33::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &NI::Matrix33::operator-;
			usertypeDefinition[sol::meta_function::equal_to] = &NI::Matrix33::operator==;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Matrix33(const float) const>(&NI::Matrix33::operator*),
				sol::resolve<NI::Vector3(const NI::Vector3&) const>(&NI::Matrix33::operator*),
				sol::resolve<NI::Matrix33(const NI::Matrix33&) const>(&NI::Matrix33::operator*)
			);

			// Operator overloading.
			usertypeDefinition[sol::meta_function::to_string] = &NI::Matrix33::toString;
			usertypeDefinition["__tojson"] = &NI::Matrix33::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &NI::Matrix33::m0;
			usertypeDefinition["y"] = &NI::Matrix33::m1;
			usertypeDefinition["z"] = &NI::Matrix33::m2;

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::Matrix33::copy;
			usertypeDefinition["fromEulerXYZ"] = &NI::Matrix33::fromEulerXYZ;
			usertypeDefinition["fromQuaternion"] = &NI::Matrix33::fromQuaternion;
			usertypeDefinition["reorthogonalize"] = &NI::Matrix33::reorthogonalize;
			usertypeDefinition["toIdentity"] = &NI::Matrix33::toIdentity;
			usertypeDefinition["toRotation"] = &NI::Matrix33::toRotation;
			usertypeDefinition["toRotationX"] = &NI::Matrix33::toRotationX;
			usertypeDefinition["toRotationY"] = &NI::Matrix33::toRotationY;
			usertypeDefinition["toRotationZ"] = &NI::Matrix33::toRotationZ;
			usertypeDefinition["toZero"] = &NI::Matrix33::toZero;
			usertypeDefinition["transpose"] = &NI::Matrix33::transpose;
			usertypeDefinition["lookAt"] = &NI::Matrix33::lookAt;

			// Handle functions with out values.
			usertypeDefinition["invert"] = &NI::Matrix33::invert_lua;
			usertypeDefinition["toEulerXYZ"] = &NI::Matrix33::toEulerXYZ_lua;
			usertypeDefinition["toEulerZYX"] = &NI::Matrix33::toEulerZYX_lua;
			usertypeDefinition["toQuaternion"] = &NI::Matrix33::toQuaternion;
			usertypeDefinition["getForwardVector"] = &NI::Matrix33::getForwardVector;
			usertypeDefinition["getRightVector"] = &NI::Matrix33::getRightVector;
			usertypeDefinition["getUpVector"] = &NI::Matrix33::getUpVector;

			// Alternate constructors.
			usertypeDefinition["identity"] = sol_copy_wrapper(NI::Matrix33::IDENTITY);
		}

		// Binding for NI::Matrix44.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Matrix44>("tes3matrix44");
			usertypeDefinition["new"] = sol::constructors<
				NI::Matrix44(),
				NI::Matrix44(const NI::Vector4&, const NI::Vector4&, const NI::Vector4&, const NI::Vector4&),
				NI::Matrix44(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &NI::Matrix44::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &NI::Matrix44::operator-;
			usertypeDefinition[sol::meta_function::equal_to] = &NI::Matrix44::operator==;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Matrix44(const float)>(&NI::Matrix44::operator*),
				sol::resolve<NI::Matrix44(const NI::Matrix44&)>(&NI::Matrix44::operator*)
			);

			// Operator overloading.
			usertypeDefinition[sol::meta_function::to_string] = &NI::Matrix44::toString;
			usertypeDefinition["__tojson"] = &NI::Matrix44::toJson;

			// Basic property bindings.
			usertypeDefinition["w"] = &NI::Matrix44::m0;
			usertypeDefinition["x"] = &NI::Matrix44::m1;
			usertypeDefinition["y"] = &NI::Matrix44::m2;
			usertypeDefinition["z"] = &NI::Matrix44::m3;

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::Matrix44::copy;
			usertypeDefinition["toZero"] = &NI::Matrix44::toZero;

			// Alternate constructors.
			usertypeDefinition["identity"] = sol_copy_wrapper(NI::Matrix44::IDENTITY);
		}

		// Binding for NI::Transform.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Transform>("tes3transform");
			usertypeDefinition["new"] = sol::constructors<
				NI::Transform(),
				NI::Transform(const NI::Matrix33& rotation, const NI::Vector3& translation, const float scale)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Transform(const NI::Transform&) const>(&NI::Transform::operator*),
				sol::resolve<NI::Vector3(const NI::Vector3&) const>(&NI::Transform::operator*)
			);

			// Basic property bindings.
			usertypeDefinition["rotation"] = &NI::Transform::rotation;
			usertypeDefinition["translation"] = &NI::Transform::translation;
			usertypeDefinition["scale"] = &NI::Transform::scale;

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::Transform::copy;
			usertypeDefinition["invert"] = sol::resolve<std::tuple<NI::Transform, bool>() const>(&NI::Transform::invert);
			usertypeDefinition["toIdentity"] = &NI::Transform::toIdentity;
		}
	}
}
