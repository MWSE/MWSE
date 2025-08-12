#include "TES3Vectors.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Vectors.h"

#include "NIColor.h"
#include "NIQuaternion.h"

namespace mwse::lua {
	void bindTES3Vectors() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::Range<int>.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Range<int>>("tes3rangeInt");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property bindings.
			usertypeDefinition["min"] = &TES3::Range<int>::min;
			usertypeDefinition["max"] = &TES3::Range<int>::max;
		}

		// Binding for TES3::Vector2.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Vector2>("tes3vector2");
			usertypeDefinition["new"] = sol::constructors<TES3::Vector2(), TES3::Vector2(float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &TES3::Vector2::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &TES3::Vector2::operator-;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<TES3::Vector2(const TES3::Vector2&) const>(&TES3::Vector2::operator*),
				sol::resolve<TES3::Vector2(const float) const>(&TES3::Vector2::operator*)
			);
			usertypeDefinition[sol::meta_function::division] = &TES3::Vector2::operator/;
			usertypeDefinition[sol::meta_function::length] = &TES3::Vector2::length;
			usertypeDefinition[sol::meta_function::to_string] = &TES3::Vector2::toString;

			// Allow objects to be serialized to json using their ID.
			usertypeDefinition["__tojson"] = &TES3::Vector2::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &TES3::Vector2::x;
			usertypeDefinition["y"] = &TES3::Vector2::y;

			// Basic function binding.
			usertypeDefinition["copy"] = &TES3::Vector2::copy;
			usertypeDefinition["distance"] = &TES3::Vector2::distance;
			usertypeDefinition["distanceChebyshev"] = &TES3::Vector2::distanceChebyshev;
			usertypeDefinition["distanceManhattan"] = &TES3::Vector2::distanceManhattan;
			usertypeDefinition["length"] = &TES3::Vector2::length;
			usertypeDefinition["normalize"] = &TES3::Vector2::normalize;
			usertypeDefinition["normalized"] = &TES3::Vector2::normalized;
			usertypeDefinition["min"] = &TES3::Vector2::min;
			usertypeDefinition["max"] = &TES3::Vector2::max;

			// Alternate constructors.
			usertypeDefinition["unitX"] = sol_copy_wrapper(TES3::Vector2::UNIT_X);
			usertypeDefinition["unitY"] = sol_copy_wrapper(TES3::Vector2::UNIT_Y);
			usertypeDefinition["ones"] = sol_copy_wrapper(TES3::Vector2::ONES);
			usertypeDefinition["zeroes"] = sol_copy_wrapper(TES3::Vector2::ZEROES);
		}

		// Binding for TES3::Vector3.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Vector3>("tes3vector3");
			usertypeDefinition["new"] = sol::constructors<TES3::Vector3(), TES3::Vector3(float, float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = sol::overload(
				sol::resolve<TES3::Vector3(const TES3::Vector3&) const>(&TES3::Vector3::operator+),
				sol::resolve<TES3::Vector3(const float) const>(&TES3::Vector3::operator+)
			);
			usertypeDefinition[sol::meta_function::subtraction] = sol::overload(
				sol::resolve<TES3::Vector3(const TES3::Vector3&) const>(&TES3::Vector3::operator-),
				sol::resolve<TES3::Vector3(const float) const>(&TES3::Vector3::operator-)
			);
			usertypeDefinition[sol::meta_function::unary_minus] = sol::resolve<TES3::Vector3() const>(&TES3::Vector3::operator-);;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<TES3::Vector3(const TES3::Vector3&) const>(&TES3::Vector3::operator*),
				sol::resolve<TES3::Vector3(const float) const>(&TES3::Vector3::operator*)
			);
			usertypeDefinition[sol::meta_function::division] = &TES3::Vector3::operator/;
			usertypeDefinition[sol::meta_function::length] = &TES3::Vector3::length;
			usertypeDefinition[sol::meta_function::to_string] = &TES3::Vector3::toString;

			// Allow objects to be serialized to json using their ID.
			usertypeDefinition["__tojson"] = &TES3::Vector3::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &TES3::Vector3::x;
			usertypeDefinition["y"] = &TES3::Vector3::y;
			usertypeDefinition["z"] = &TES3::Vector3::z;

			// These can also be used for RGB.
			usertypeDefinition["r"] = &TES3::Vector3::x;
			usertypeDefinition["g"] = &TES3::Vector3::y;
			usertypeDefinition["b"] = &TES3::Vector3::z;

			// Basic function binding.
			usertypeDefinition["angle"] = &TES3::Vector3::angle;
			usertypeDefinition["copy"] = &TES3::Vector3::copy;
			usertypeDefinition["cross"] = &TES3::Vector3::crossProduct;
			usertypeDefinition["distance"] = &TES3::Vector3::distance;
			usertypeDefinition["distanceChebyshev"] = &TES3::Vector3::distanceChebyshev;
			usertypeDefinition["distanceManhattan"] = &TES3::Vector3::distanceManhattan;
			usertypeDefinition["distanceXY"] = &TES3::Vector3::distanceXY;
			usertypeDefinition["dot"] = &TES3::Vector3::dotProduct;
			usertypeDefinition["outerProduct"] = &TES3::Vector3::outerProduct;
			usertypeDefinition["heightDifference"] = &TES3::Vector3::heightDifference;
			usertypeDefinition["length"] = &TES3::Vector3::length;
			usertypeDefinition["lerp"] = &TES3::Vector3::lerp;
			usertypeDefinition["negate"] = &TES3::Vector3::negate;
			usertypeDefinition["normalize"] = &TES3::Vector3::normalize;
			usertypeDefinition["normalized"] = &TES3::Vector3::normalized;
			usertypeDefinition["interpolate"] = &TES3::Vector3::interpolate;
			usertypeDefinition["min"] = &TES3::Vector3::min;
			usertypeDefinition["max"] = &TES3::Vector3::max;

			// Conversion to NI::Color.
			usertypeDefinition["toColor"] = &TES3::Vector3::toNiColor;

			// Alternate constructors.
			usertypeDefinition["unitX"] = sol_copy_wrapper(TES3::Vector3::UNIT_X);
			usertypeDefinition["unitY"] = sol_copy_wrapper(TES3::Vector3::UNIT_Y);
			usertypeDefinition["unitZ"] = sol_copy_wrapper(TES3::Vector3::UNIT_Z);
			usertypeDefinition["ones"] = sol_copy_wrapper(TES3::Vector3::ONES);
			usertypeDefinition["zeroes"] = sol_copy_wrapper(TES3::Vector3::ZEROES);
		}

		// Binding for TES3::Vector4.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Vector4>("tes3vector4");
			usertypeDefinition["new"] = sol::constructors<TES3::Vector4(), TES3::Vector4(float, float, float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &TES3::Vector4::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &TES3::Vector4::operator-;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<TES3::Vector4(const TES3::Vector4&) const>(&TES3::Vector4::operator*),
				sol::resolve<TES3::Vector4(const float) const>(&TES3::Vector4::operator*)
			);
			usertypeDefinition[sol::meta_function::division] = &TES3::Vector4::operator/;
			usertypeDefinition[sol::meta_function::length] = &TES3::Vector4::length;
			usertypeDefinition[sol::meta_function::to_string] = &TES3::Vector4::toString;
			usertypeDefinition["__tojson"] = &TES3::Vector4::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &TES3::Vector4::x;
			usertypeDefinition["y"] = &TES3::Vector4::y;
			usertypeDefinition["z"] = &TES3::Vector4::z;
			usertypeDefinition["w"] = &TES3::Vector4::w;

			// Basic function binding.
			usertypeDefinition["copy"] = &TES3::Vector4::copy;
			usertypeDefinition["distance"] = &TES3::Vector4::distance;
			usertypeDefinition["distanceChebyshev"] = &TES3::Vector4::distanceChebyshev;
			usertypeDefinition["distanceManhattan"] = &TES3::Vector4::distanceManhattan;
			usertypeDefinition["length"] = &TES3::Vector4::length;
			usertypeDefinition["min"] = &TES3::Vector4::min;
			usertypeDefinition["max"] = &TES3::Vector4::max;
		}

		// Binding for TES3::BoundingBox.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::BoundingBox>("tes3boundingBox");
			usertypeDefinition["new"] = sol::no_constructor;

			// Operator overloading.
			usertypeDefinition[sol::meta_function::to_string] = &TES3::BoundingBox::toString;

			// Allow objects to be serialized to json using their ID.
			usertypeDefinition["__tojson"] = &TES3::BoundingBox::toJson;

			// Basic property bindings.
			usertypeDefinition["max"] = &TES3::BoundingBox::maximum;
			usertypeDefinition["min"] = &TES3::BoundingBox::minimum;

			// Basic function binding.
			usertypeDefinition["copy"] = &TES3::BoundingBox::copy;
			usertypeDefinition["vertices"] = &TES3::BoundingBox::vertices;
		}

		// Binding for TES3::Matrix33.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Matrix33>("tes3matrix33");
			usertypeDefinition["new"] = sol::constructors<
				TES3::Matrix33(),
				TES3::Matrix33(TES3::Vector3*, TES3::Vector3*, TES3::Vector3*),
				TES3::Matrix33(float, float, float, float, float, float, float, float, float)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &TES3::Matrix33::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &TES3::Matrix33::operator-;
			usertypeDefinition[sol::meta_function::equal_to] = &TES3::Matrix33::operator==;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<TES3::Matrix33(const float)>(&TES3::Matrix33::operator*),
				sol::resolve<TES3::Vector3(const TES3::Vector3&) const>(&TES3::Matrix33::operator*),
				sol::resolve<TES3::Matrix33(const TES3::Matrix33&)>(&TES3::Matrix33::operator*)
			);

			// Operator overloading.
			usertypeDefinition[sol::meta_function::to_string] = &TES3::Matrix33::toString;
			usertypeDefinition["__tojson"] = &TES3::Matrix33::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &TES3::Matrix33::m0;
			usertypeDefinition["y"] = &TES3::Matrix33::m1;
			usertypeDefinition["z"] = &TES3::Matrix33::m2;

			// Basic function binding.
			usertypeDefinition["copy"] = &TES3::Matrix33::copy;
			usertypeDefinition["fromEulerXYZ"] = &TES3::Matrix33::fromEulerXYZ;
			usertypeDefinition["fromQuaternion"] = &TES3::Matrix33::fromQuaternion;
			usertypeDefinition["reorthogonalize"] = &TES3::Matrix33::reorthogonalize;
			usertypeDefinition["toIdentity"] = &TES3::Matrix33::toIdentity;
			usertypeDefinition["toRotation"] = &TES3::Matrix33::toRotation;
			usertypeDefinition["toRotationX"] = &TES3::Matrix33::toRotationX;
			usertypeDefinition["toRotationY"] = &TES3::Matrix33::toRotationY;
			usertypeDefinition["toRotationZ"] = &TES3::Matrix33::toRotationZ;
			usertypeDefinition["toZero"] = &TES3::Matrix33::toZero;
			usertypeDefinition["transpose"] = &TES3::Matrix33::transpose;
			usertypeDefinition["lookAt"] = &TES3::Matrix33::lookAt;

			// Handle functions with out values.
			usertypeDefinition["invert"] = &TES3::Matrix33::invert_lua;
			usertypeDefinition["toEulerXYZ"] = &TES3::Matrix33::toEulerXYZ_lua;
			usertypeDefinition["toEulerZYX"] = &TES3::Matrix33::toEulerZYX_lua;
			usertypeDefinition["toQuaternion"] = &TES3::Matrix33::toQuaternion;
			usertypeDefinition["getForwardVector"] = &TES3::Matrix33::getForwardVector;
			usertypeDefinition["getRightVector"] = &TES3::Matrix33::getRightVector;
			usertypeDefinition["getUpVector"] = &TES3::Matrix33::getUpVector;

			// Alternate constructors.
			usertypeDefinition["identity"] = sol_copy_wrapper(TES3::Matrix33::IDENTITY);
		}

		// Binding for TES3::Matrix44.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Matrix44>("tes3matrix44");
			usertypeDefinition["new"] = sol::constructors<
				TES3::Matrix44(),
				TES3::Matrix44(const TES3::Vector4&, const TES3::Vector4&, const TES3::Vector4&, const TES3::Vector4&),
				TES3::Matrix44(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &TES3::Matrix44::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &TES3::Matrix44::operator-;
			usertypeDefinition[sol::meta_function::equal_to] = &TES3::Matrix44::operator==;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<TES3::Matrix44(const float)>(&TES3::Matrix44::operator*),
				sol::resolve<TES3::Matrix44(const TES3::Matrix44&)>(&TES3::Matrix44::operator*)
			);

			// Operator overloading.
			usertypeDefinition[sol::meta_function::to_string] = &TES3::Matrix44::toString;
			usertypeDefinition["__tojson"] = &TES3::Matrix44::toJson;

			// Basic property bindings.
			usertypeDefinition["w"] = &TES3::Matrix44::m0;
			usertypeDefinition["x"] = &TES3::Matrix44::m1;
			usertypeDefinition["y"] = &TES3::Matrix44::m2;
			usertypeDefinition["z"] = &TES3::Matrix44::m3;

			// Basic function binding.
			usertypeDefinition["copy"] = &TES3::Matrix44::copy;
			usertypeDefinition["toZero"] = &TES3::Matrix44::toZero;

			// Alternate constructors.
			usertypeDefinition["identity"] = sol_copy_wrapper(TES3::Matrix44::IDENTITY);
		}

		// Binding for TES3::Transform.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Transform>("tes3transform");
			usertypeDefinition["new"] = sol::constructors<
				TES3::Transform(),
				TES3::Transform(const TES3::Matrix33& rotation, const TES3::Vector3& translation, const float scale)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<TES3::Transform(const TES3::Transform&)>(&TES3::Transform::operator*),
				sol::resolve<TES3::Vector3(const TES3::Vector3&)>(&TES3::Transform::operator*)
			);

			// Basic property bindings.
			usertypeDefinition["rotation"] = &TES3::Transform::rotation;
			usertypeDefinition["translation"] = &TES3::Transform::translation;
			usertypeDefinition["scale"] = &TES3::Transform::scale;

			// Basic function binding.
			usertypeDefinition["copy"] = &TES3::Transform::copy;
			usertypeDefinition["invert"] = sol::resolve<std::tuple<TES3::Transform, bool>() const>(&TES3::Transform::invert);
			usertypeDefinition["toIdentity"] = &TES3::Transform::toIdentity;
		}
	}
}
