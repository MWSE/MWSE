#include "NIColorLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIObjectLua.h"

#include "NIAnimationData.h"
#include "NIColor.h"

namespace mwse::lua {
	void bindNIColor() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// NiPackedColor
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::PackedColor>("niPackedColor");
			usertypeDefinition["new"] = sol::constructors<
				NI::PackedColor(),
				NI::PackedColor(unsigned char, unsigned char, unsigned char),
				NI::PackedColor(unsigned char, unsigned char, unsigned char, unsigned char)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::to_string] = &NI::PackedColor::toString;

			// Basic property binding.
			usertypeDefinition["r"] = &NI::PackedColor::r;
			usertypeDefinition["g"] = &NI::PackedColor::g;
			usertypeDefinition["b"] = &NI::PackedColor::b;
			usertypeDefinition["a"] = &NI::PackedColor::a;

			// Long aliases for color access.
			usertypeDefinition["red"] = &NI::PackedColor::r;
			usertypeDefinition["green"] = &NI::PackedColor::g;
			usertypeDefinition["blue"] = &NI::PackedColor::b;
			usertypeDefinition["alpha"] = &NI::PackedColor::a;
		}

		// NiColor
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Color>("niColor");
			usertypeDefinition["new"] = sol::constructors<NI::Color(), NI::Color(float, float, float)>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::addition] = &NI::Color::operator+;
			usertypeDefinition[sol::meta_function::subtraction] = &NI::Color::operator-;
			usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
				sol::resolve<NI::Color(const NI::Color&)>(&NI::Color::operator*),
				sol::resolve<NI::Color(const float)>(&NI::Color::operator*)
			);
			usertypeDefinition[sol::meta_function::to_string] = &NI::Color::toString;
			usertypeDefinition["__tojson"] = &NI::Color::toJson;

			// Basic property binding.
			usertypeDefinition["r"] = &NI::Color::r;
			usertypeDefinition["g"] = &NI::Color::g;
			usertypeDefinition["b"] = &NI::Color::b;

			// Long aliases for color access.
			usertypeDefinition["red"] = &NI::Color::r;
			usertypeDefinition["green"] = &NI::Color::g;
			usertypeDefinition["blue"] = &NI::Color::b;

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::Color::copy;
			usertypeDefinition["lerp"] = &NI::Color::lerp;
			usertypeDefinition["clamp"] = &NI::Color::clamp;

			// Conversion to TES3::Vector3.
			usertypeDefinition["toVector3"] = &NI::Color::toVector3;
		}

		// NiColorA
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ColorA>("niColorA");
			usertypeDefinition["new"] = sol::constructors<
				NI::ColorA(),
				NI::ColorA(float, float, float),
				NI::ColorA(float, float, float, float)
			>();

			// Operator overloading.
			usertypeDefinition[sol::meta_function::to_string] = &NI::ColorA::toString;

			// Basic property binding.
			usertypeDefinition["r"] = &NI::ColorA::r;
			usertypeDefinition["g"] = &NI::ColorA::g;
			usertypeDefinition["b"] = &NI::ColorA::b;
			usertypeDefinition["a"] = &NI::ColorA::a;

			// Long aliases for colorA access.
			usertypeDefinition["red"] = &NI::ColorA::r;
			usertypeDefinition["green"] = &NI::ColorA::g;
			usertypeDefinition["blue"] = &NI::ColorA::b;
			usertypeDefinition["alpha"] = &NI::ColorA::a;

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::ColorA::copy;
			usertypeDefinition["lerp"] = &NI::ColorA::lerp;
		}

		// NiColorData
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ColorData>("niColorData");

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
			setUserdataForNIObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["keyCount"] = &NI::ColorData::keyCount;
			usertypeDefinition["keys"] = sol::readonly_property(&NI::ColorData::getKeys);
			usertypeDefinition["keyType"] = &NI::ColorData::keyType;
		}

		// NiColorKey
		// TODO: Update this when the AnimationKey defines are in.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ColorKey>("niColorKey");

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::AnimationKey>();
			usertypeDefinition["timing"] = &NI::ColorKey::timing;

			// Basic property binding.
			usertypeDefinition["color"] = &NI::ColorKey::color;
		}
	}
}
