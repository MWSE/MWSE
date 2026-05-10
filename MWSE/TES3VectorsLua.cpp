#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIColor.h"
#include "NIMatrix33.h"
#include "NIMatrix44.h"
#include "NIRange.h"
#include "NIQuaternion.h"
#include "NIPoint2.h"
#include "NIPoint3.h"
#include "NIPoint4.h"

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

		// Binding for NI::Point2.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Point2>("tes3vector2");
			usertypeDefinition["new"] = sol::constructors<NI::Point2(), NI::Point2(float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &NI::Point2::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &NI::Point2::operator-;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Point2(const NI::Point2&) const>(&NI::Point2::operator*),
				sol::resolve<NI::Point2(const float) const>(&NI::Point2::operator*)
			);
			usertypeDefinition[sol::meta_function::division] = &NI::Point2::operator/;
			usertypeDefinition[sol::meta_function::length] = &NI::Point2::length;
			usertypeDefinition[sol::meta_function::to_string] = &NI::Point2::toString;

			// Allow objects to be serialized to json using their ID.
			usertypeDefinition["__tojson"] = &NI::Point2::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &NI::Point2::x;
			usertypeDefinition["y"] = &NI::Point2::y;

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::Point2::copy;
			usertypeDefinition["distance"] = &NI::Point2::distance;
			usertypeDefinition["distanceChebyshev"] = &NI::Point2::distanceChebyshev;
			usertypeDefinition["distanceManhattan"] = &NI::Point2::distanceManhattan;
			usertypeDefinition["length"] = &NI::Point2::length;
			usertypeDefinition["normalize"] = &NI::Point2::normalize;
			usertypeDefinition["normalized"] = &NI::Point2::normalized;
			usertypeDefinition["min"] = &NI::Point2::min;
			usertypeDefinition["max"] = &NI::Point2::max;

			// Alternate constructors.
			usertypeDefinition["unitX"] = sol_copy_wrapper(NI::Point2::UNIT_X);
			usertypeDefinition["unitY"] = sol_copy_wrapper(NI::Point2::UNIT_Y);
			usertypeDefinition["ones"] = sol_copy_wrapper(NI::Point2::ONES);
			usertypeDefinition["zeroes"] = sol_copy_wrapper(NI::Point2::ZEROES);
		}

		// Binding for NI::Point3.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Point3>("tes3vector3");
			usertypeDefinition["new"] = sol::constructors<NI::Point3(), NI::Point3(float, float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = sol::overload(
				sol::resolve<NI::Point3(const NI::Point3&) const>(&NI::Point3::operator+),
				sol::resolve<NI::Point3(const float) const>(&NI::Point3::operator+)
			);
			usertypeDefinition[sol::meta_function::subtraction] = sol::overload(
				sol::resolve<NI::Point3(const NI::Point3&) const>(&NI::Point3::operator-),
				sol::resolve<NI::Point3(const float) const>(&NI::Point3::operator-)
			);
			usertypeDefinition[sol::meta_function::unary_minus] = sol::resolve<NI::Point3() const>(&NI::Point3::operator-);;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Point3(const NI::Point3&) const>(&NI::Point3::operator*),
				sol::resolve<NI::Point3(const float) const>(&NI::Point3::operator*)
			);
			usertypeDefinition[sol::meta_function::division] = &NI::Point3::operator/;
			usertypeDefinition[sol::meta_function::length] = &NI::Point3::length;
			usertypeDefinition[sol::meta_function::to_string] = &NI::Point3::toString;

			// Allow objects to be serialized to json using their ID.
			usertypeDefinition["__tojson"] = &NI::Point3::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &NI::Point3::x;
			usertypeDefinition["y"] = &NI::Point3::y;
			usertypeDefinition["z"] = &NI::Point3::z;

			// These can also be used for RGB.
			usertypeDefinition["r"] = &NI::Point3::x;
			usertypeDefinition["g"] = &NI::Point3::y;
			usertypeDefinition["b"] = &NI::Point3::z;

			// Basic function binding.
			usertypeDefinition["angle"] = &NI::Point3::angle;
			usertypeDefinition["copy"] = &NI::Point3::copy;
			usertypeDefinition["cross"] = &NI::Point3::crossProduct;
			usertypeDefinition["distance"] = &NI::Point3::distance;
			usertypeDefinition["distanceChebyshev"] = &NI::Point3::distanceChebyshev;
			usertypeDefinition["distanceManhattan"] = &NI::Point3::distanceManhattan;
			usertypeDefinition["distanceXY"] = &NI::Point3::distanceXY;
			usertypeDefinition["dot"] = &NI::Point3::dotProduct;
			usertypeDefinition["outerProduct"] = &NI::Point3::outerProduct;
			usertypeDefinition["heightDifference"] = &NI::Point3::heightDifference;
			usertypeDefinition["length"] = &NI::Point3::length;
			usertypeDefinition["lerp"] = &NI::Point3::lerp;
			usertypeDefinition["negate"] = &NI::Point3::negate;
			usertypeDefinition["normalize"] = &NI::Point3::normalize;
			usertypeDefinition["normalized"] = &NI::Point3::normalized;
			usertypeDefinition["interpolate"] = &NI::Point3::interpolate;
			usertypeDefinition["min"] = &NI::Point3::min;
			usertypeDefinition["max"] = &NI::Point3::max;

			// Conversion to NI::Color.
			usertypeDefinition["toColor"] = &NI::Point3::toNiColor;

			// Alternate constructors.
			usertypeDefinition["unitX"] = sol_copy_wrapper(NI::Point3::UNIT_X);
			usertypeDefinition["unitY"] = sol_copy_wrapper(NI::Point3::UNIT_Y);
			usertypeDefinition["unitZ"] = sol_copy_wrapper(NI::Point3::UNIT_Z);
			usertypeDefinition["ones"] = sol_copy_wrapper(NI::Point3::ONES);
			usertypeDefinition["zeroes"] = sol_copy_wrapper(NI::Point3::ZEROES);
		}

		// Binding for NI::Point4.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Point4>("tes3vector4");
			usertypeDefinition["new"] = sol::constructors<NI::Point4(), NI::Point4(float, float, float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &NI::Point4::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &NI::Point4::operator-;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Point4(const NI::Point4&) const>(&NI::Point4::operator*),
				sol::resolve<NI::Point4(const float) const>(&NI::Point4::operator*)
			);
			usertypeDefinition[sol::meta_function::division] = &NI::Point4::operator/;
			usertypeDefinition[sol::meta_function::length] = &NI::Point4::length;
			usertypeDefinition[sol::meta_function::to_string] = &NI::Point4::toString;
			usertypeDefinition["__tojson"] = &NI::Point4::toJson;

			// Basic property bindings.
			usertypeDefinition["x"] = &NI::Point4::x;
			usertypeDefinition["y"] = &NI::Point4::y;
			usertypeDefinition["z"] = &NI::Point4::z;
			usertypeDefinition["w"] = &NI::Point4::w;

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::Point4::copy;
			usertypeDefinition["distance"] = &NI::Point4::distance;
			usertypeDefinition["distanceChebyshev"] = &NI::Point4::distanceChebyshev;
			usertypeDefinition["distanceManhattan"] = &NI::Point4::distanceManhattan;
			usertypeDefinition["length"] = &NI::Point4::length;
			usertypeDefinition["min"] = &NI::Point4::min;
			usertypeDefinition["max"] = &NI::Point4::max;
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
				NI::Matrix33(NI::Point3*, NI::Point3*, NI::Point3*),
				NI::Matrix33(float, float, float, float, float, float, float, float, float)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &NI::Matrix33::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &NI::Matrix33::operator-;
			usertypeDefinition[sol::meta_function::equal_to] = &NI::Matrix33::operator==;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Matrix33(const float) const>(&NI::Matrix33::operator*),
				sol::resolve<NI::Point3(const NI::Point3&) const>(&NI::Matrix33::operator*),
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
				NI::Matrix44(const NI::Point4&, const NI::Point4&, const NI::Point4&, const NI::Point4&),
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
				NI::Transform(const NI::Matrix33& rotation, const NI::Point3& translation, const float scale)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Transform(const NI::Transform&) const>(&NI::Transform::operator*),
				sol::resolve<NI::Point3(const NI::Point3&) const>(&NI::Transform::operator*)
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
