#include "TES3Vectors.h"

#include "sol.hpp"
#include "LuaManager.h"

#include "TES3Vectors.h"

#include <iomanip>

namespace mwse {
	namespace lua {
		void bindTES3Vectors() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for TES3::Range<int>.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::Range<int>>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property bindings.
				usertypeDefinition.set("min", &TES3::Range<int>::min);
				usertypeDefinition.set("max", &TES3::Range<int>::max);

				// Finish up our usertype.
				state.set_usertype("tes3rangeInt", usertypeDefinition);
			}

			// Binding for TES3::Vector2.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::Vector2>();
				usertypeDefinition.set("new", sol::constructors<TES3::Vector2(), TES3::Vector2(float, float)>());

				// Basic property bindings.
				usertypeDefinition.set("x", &TES3::Vector2::x);
				usertypeDefinition.set("y", &TES3::Vector2::y);

				// Basic function binding.
				usertypeDefinition.set("copy", [](TES3::Vector2& self) { return TES3::Vector2(self); });

				// Finish up our usertype.
				state.set_usertype("tes3vector2", usertypeDefinition);
			}

			// Binding for TES3::Vector3.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::Vector3>();
				usertypeDefinition.set("new", sol::constructors<TES3::Vector3(), TES3::Vector3(float, float, float)>());

				// Operator overloading.
				usertypeDefinition.set(sol::meta_function::addition, &TES3::Vector3::operator+);
				usertypeDefinition.set(sol::meta_function::subtraction, &TES3::Vector3::operator-);
				usertypeDefinition.set(sol::meta_function::multiplication, sol::overload(
					sol::resolve<TES3::Vector3(const TES3::Vector3&)>(&TES3::Vector3::operator*),
					sol::resolve<TES3::Vector3(const float)>(&TES3::Vector3::operator*)
				));
				usertypeDefinition.set(sol::meta_function::length, &TES3::Vector3::length);
				usertypeDefinition.set(sol::meta_function::to_string, [](TES3::Vector3& self)
				{
					std::ostringstream ss;
					ss << std::fixed << std::setprecision(2) << "<" << self.x << ", " << self.y << ", " << self.z << ">";
					return ss.str();
				}
				);

				// Basic property bindings.
				usertypeDefinition.set("x", &TES3::Vector3::x);
				usertypeDefinition.set("y", &TES3::Vector3::y);
				usertypeDefinition.set("z", &TES3::Vector3::z);

				// These can also be used for RGB.
				usertypeDefinition.set("r", &TES3::Vector3::x);
				usertypeDefinition.set("g", &TES3::Vector3::y);
				usertypeDefinition.set("b", &TES3::Vector3::z);

				// Basic function binding.
				usertypeDefinition.set("copy", [](TES3::Vector3& self) { return TES3::Vector3(self); });
				usertypeDefinition.set("cross", &TES3::Vector3::crossProduct);
				usertypeDefinition.set("distance", &TES3::Vector3::distance);
				usertypeDefinition.set("dot", &TES3::Vector3::dotProduct);
				usertypeDefinition.set("heightDifference", &TES3::Vector3::heightDifference);
				usertypeDefinition.set("length", &TES3::Vector3::length);
				usertypeDefinition.set("negate", &TES3::Vector3::negate);

				// Finish up our usertype.
				state.set_usertype("tes3vector3", usertypeDefinition);
			}

			// Binding for TES3::Vector4.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::Vector4>();
				usertypeDefinition.set("new", sol::constructors<TES3::Vector4(), TES3::Vector4(float, float, float, float)>());

				// Basic property bindings.
				usertypeDefinition.set("x", &TES3::Vector4::x);
				usertypeDefinition.set("y", &TES3::Vector4::y);
				usertypeDefinition.set("z", &TES3::Vector4::z);
				usertypeDefinition.set("w", &TES3::Vector4::w);

				// Basic function binding.
				usertypeDefinition.set("copy", [](TES3::Vector4& self) { return TES3::Vector4(self); });

				// Finish up our usertype.
				state.set_usertype("tes3vector4", usertypeDefinition);
			}

			// Binding for TES3::BoundingBox.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::BoundingBox>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property bindings.
				usertypeDefinition.set("max", &TES3::BoundingBox::maximum);
				usertypeDefinition.set("min", &TES3::BoundingBox::minimum);

				// Basic function binding.
				usertypeDefinition.set("copy", [](TES3::BoundingBox& self) { return TES3::BoundingBox(self); });

				// Finish up our usertype.
				state.set_usertype("tes3boundingBox", usertypeDefinition);
			}

			// Binding for TES3::Matrix33.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::Matrix33>();
				usertypeDefinition.set("new", sol::constructors<
						TES3::Matrix33(), 
						TES3::Matrix33(TES3::Vector3*, TES3::Vector3*, TES3::Vector3*),
						TES3::Matrix33(float, float, float, float, float, float, float, float, float)
				>());

				// Operator overloading.
				usertypeDefinition.set(sol::meta_function::addition, &TES3::Matrix33::operator+);
				usertypeDefinition.set(sol::meta_function::subtraction, &TES3::Matrix33::operator-);
				usertypeDefinition.set(sol::meta_function::equal_to, &TES3::Matrix33::operator==);
				usertypeDefinition.set(sol::meta_function::multiplication, sol::overload(
					sol::resolve<TES3::Matrix33(const float)>(&TES3::Matrix33::operator*),
					sol::resolve<TES3::Vector3(const TES3::Vector3&)>(&TES3::Matrix33::operator*),
					sol::resolve<TES3::Matrix33(const TES3::Matrix33&)>(&TES3::Matrix33::operator*)
				));

				// Operator overloading.
				usertypeDefinition.set(sol::meta_function::to_string, [](TES3::Matrix33& self) {
					std::ostringstream ss;
					ss << std::fixed << std::setprecision(2) << std::dec
						<< "<<<" << self.m0.x << ", " << self.m0.y << ", " << self.m0.z << ">,\n"
						<< "  <" << self.m1.x << ", " << self.m1.y << ", " << self.m1.z << ">,\n"
						<< "  <" << self.m2.x << ", " << self.m2.y << ", " << self.m2.z << ">>>";
					return ss.str();
				});

				// Basic property bindings.
				usertypeDefinition.set("x", sol::readonly_property([](TES3::Matrix33& self) { return self.m0; }));
				usertypeDefinition.set("y", sol::readonly_property([](TES3::Matrix33& self) { return self.m1; }));
				usertypeDefinition.set("z", sol::readonly_property([](TES3::Matrix33& self) { return self.m2; }));

				// Basic function binding.
				usertypeDefinition.set("copy", [](TES3::Matrix33& self) { return TES3::Matrix33(self); });
				usertypeDefinition.set("fromEulerXYZ", &TES3::Matrix33::fromEulerXYZ);
				usertypeDefinition.set("reorthogonalize", &TES3::Matrix33::reorthogonalize);
				usertypeDefinition.set("toIdentity", &TES3::Matrix33::toIdentity);
				usertypeDefinition.set("toRotation", &TES3::Matrix33::toRotation);
				usertypeDefinition.set("toRotationX", &TES3::Matrix33::toRotationX);
				usertypeDefinition.set("toRotationY", &TES3::Matrix33::toRotationY);
				usertypeDefinition.set("toRotationZ", &TES3::Matrix33::toRotationZ);
				usertypeDefinition.set("toZero", &TES3::Matrix33::toZero);
				usertypeDefinition.set("transpose", &TES3::Matrix33::transpose);

				// Handle functions with out values.
				usertypeDefinition.set("invert", [](TES3::Matrix33& self) {
					TES3::Matrix33 matrix;
					bool valid = self.invert(&matrix);
					return std::make_tuple(matrix, valid);
				});
				usertypeDefinition.set("toEulerXYZ", [](TES3::Matrix33& self) {
					float x, y, z;
					bool isUnique = self.toEulerXYZ(&x, &y, &z);
					return std::make_tuple(TES3::Vector3(x, y, z), isUnique);
				});
				usertypeDefinition.set("toEulerZYX", [](TES3::Matrix33& self) {
					float x, y, z;
					bool isUnique = self.toEulerZYX(&x, &y, &z);
					return std::make_tuple(TES3::Vector3(x, y, z), isUnique);
				});

				// Finish up our usertype.
				state.set_usertype("tes3matrix33", usertypeDefinition);
			}

			// Binding for TES3::Transform.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::Transform>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property bindings.
				usertypeDefinition.set("rotation", sol::readonly_property([](TES3::Transform& self) { return self.rotation; }));
				usertypeDefinition.set("translation", sol::readonly_property([](TES3::Transform& self) { return self.translation; }));
				usertypeDefinition.set("scale", sol::readonly_property([](TES3::Transform& self) { return self.scale; }));

				// Basic function binding.
				usertypeDefinition.set("copy", [](TES3::Transform& self) { return TES3::Transform(self); });

				// Finish up our usertype.
				state.set_usertype("tes3transform", usertypeDefinition);
			}
		}
	}
}
