#include "TES3Vectors.h"

#include "LuaManager.h"

#include "TES3Vectors.h"

#include "NIColor.h"

namespace mwse {
	namespace lua {
		void bindTES3Vectors() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::Range<int>.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Range<int>>("tes3rangeInt");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property bindings.
				usertypeDefinition["min"] = &TES3::Range<int>::min;
				usertypeDefinition["max"] = &TES3::Range<int>::max;
			}

			// Binding for TES3::Vector2.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Vector2>("tes3vector2");
				usertypeDefinition["new"] = sol::constructors<TES3::Vector2(), TES3::Vector2(float, float)>();

				// Operator overloading.
				usertypeDefinition[sol::meta_function::addition] = &TES3::Vector2::operator+;
				usertypeDefinition[sol::meta_function::subtraction] = &TES3::Vector2::operator-;
				usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
					sol::resolve<TES3::Vector2(const TES3::Vector2&) const>(&TES3::Vector2::operator*),
					sol::resolve<TES3::Vector2(const float) const>(&TES3::Vector2::operator*)
				);
				usertypeDefinition[sol::meta_function::length] = &TES3::Vector2::length;
				usertypeDefinition[sol::meta_function::to_string] = &TES3::Vector2::toString;

				// Allow objects to be serialized to json using their ID.
				usertypeDefinition["__tojson"] = &TES3::Vector2::toJson;

				// Basic property bindings.
				usertypeDefinition["x"] = &TES3::Vector2::x;
				usertypeDefinition["y"] = &TES3::Vector2::y;

				// Basic function binding.
				usertypeDefinition["copy"] = &TES3::Vector2::copy;
				usertypeDefinition["length"] = &TES3::Vector2::length;
			}

			// Binding for TES3::Vector3.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Vector3>("tes3vector3");
				usertypeDefinition["new"] = sol::constructors<TES3::Vector3(), TES3::Vector3(float, float, float)>();

				// Operator overloading.
				usertypeDefinition[sol::meta_function::addition] = &TES3::Vector3::operator+;
				usertypeDefinition[sol::meta_function::subtraction] = &TES3::Vector3::operator-;
				usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
					sol::resolve<TES3::Vector3(const TES3::Vector3&) const>(&TES3::Vector3::operator*),
					sol::resolve<TES3::Vector3(const float) const>(&TES3::Vector3::operator*)
				);
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
				usertypeDefinition["dot"] = &TES3::Vector3::dotProduct;
				usertypeDefinition["outerProduct"] = &TES3::Vector3::outerProduct;
				usertypeDefinition["heightDifference"] = &TES3::Vector3::heightDifference;
				usertypeDefinition["length"] = &TES3::Vector3::length;
				usertypeDefinition["lerp"] = &TES3::Vector3::lerp;
				usertypeDefinition["negate"] = &TES3::Vector3::negate;
				usertypeDefinition["normalize"] = &TES3::Vector3::normalize;
				usertypeDefinition["normalized"] = &TES3::Vector3::normalized;
				usertypeDefinition["interpolate"] = &TES3::Vector3::interpolate;

				// Conversion to NI::Color.
				usertypeDefinition["toColor"] = &TES3::Vector3::toNiColor;
			}

			// Binding for TES3::Vector4.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Vector4>("tes3vector4");
				usertypeDefinition["new"] = sol::constructors<TES3::Vector4(), TES3::Vector4(float, float, float, float)>();

				// Operator overloading.
				usertypeDefinition[sol::meta_function::addition] = &TES3::Vector4::operator+;
				usertypeDefinition[sol::meta_function::subtraction] = &TES3::Vector4::operator-;
				usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
					sol::resolve<TES3::Vector4(const TES3::Vector4&) const>(&TES3::Vector4::operator*),
					sol::resolve<TES3::Vector4(const float) const>(&TES3::Vector4::operator*)
				);
				usertypeDefinition[sol::meta_function::length] = &TES3::Vector4::length;
				usertypeDefinition[sol::meta_function::to_string] = &TES3::Vector4::toString;
				usertypeDefinition["__tojson"] = &TES3::Vector4::toJson;

				// Basic property bindings.
				usertypeDefinition["w"] = &TES3::Vector4::w;
				usertypeDefinition["x"] = &TES3::Vector4::x;
				usertypeDefinition["y"] = &TES3::Vector4::y;
				usertypeDefinition["z"] = &TES3::Vector4::z;

				// Basic function binding.
				usertypeDefinition["copy"] = &TES3::Vector4::copy;
				usertypeDefinition["length"] = &TES3::Vector4::length;
			}

			// Binding for TES3::BoundingBox.
			{
				// Start our usertype. We must finish this with state.set_usertype.
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
			}

			// Binding for TES3::Matrix33.
			{
				// Start our usertype. We must finish this with state.set_usertype.
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
					sol::resolve<TES3::Vector3(const TES3::Vector3&)>(&TES3::Matrix33::operator*),
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
				usertypeDefinition["reorthogonalize"] = &TES3::Matrix33::reorthogonalize;
				usertypeDefinition["toIdentity"] = &TES3::Matrix33::toIdentity;
				usertypeDefinition["toRotation"] = &TES3::Matrix33::toRotation;
				usertypeDefinition["toRotationX"] = &TES3::Matrix33::toRotationX;
				usertypeDefinition["toRotationY"] = &TES3::Matrix33::toRotationY;
				usertypeDefinition["toRotationZ"] = &TES3::Matrix33::toRotationZ;
				usertypeDefinition["toZero"] = &TES3::Matrix33::toZero;
				usertypeDefinition["transpose"] = &TES3::Matrix33::transpose;

				// Handle functions with out values.
				usertypeDefinition["invert"] = &TES3::Matrix33::invert_lua;
				usertypeDefinition["toEulerXYZ"] = &TES3::Matrix33::toEulerXYZ_lua;
				usertypeDefinition["toEulerZYX"] = &TES3::Matrix33::toEulerZYX_lua;
			}

			// Binding for TES3::Matrix44.
			{
				// Start our usertype. We must finish this with state.set_usertype.
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
			}

			// Binding for TES3::Transform.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Transform>("tes3transform");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property bindings.
				usertypeDefinition["rotation"] = sol::readonly_property(&TES3::Transform::rotation);
				usertypeDefinition["translation"] = sol::readonly_property(&TES3::Transform::translation);
				usertypeDefinition["scale"] = sol::readonly_property(&TES3::Transform::scale);

				// Basic function binding.
				usertypeDefinition["copy"] = &TES3::Transform::copy;
			}
		}
	}
}
