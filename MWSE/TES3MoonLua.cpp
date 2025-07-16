#include "TES3MoonLua.h"

#include "LuaManager.h"

#include "TES3Moon.h"
#include "TES3WeatherController.h"

namespace mwse::lua {
	void bindTES3Moon() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Moon>("tes3moon");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["axisOffset"] = &TES3::Moon::axisOffset;
		usertypeDefinition["dailyIncrement"] = &TES3::Moon::dailyIncrement;
		usertypeDefinition["fadeEndAngle"] = &TES3::Moon::fadeEndAngle;
		usertypeDefinition["fadeInFinish"] = &TES3::Moon::fadeInFinish;
		usertypeDefinition["fadeInStart"] = &TES3::Moon::fadeInStart;
		usertypeDefinition["fadeOutFinish"] = &TES3::Moon::fadeOutFinish;
		usertypeDefinition["fadeOutStart"] = &TES3::Moon::fadeOutStart;
		usertypeDefinition["fadeStartAngle"] = &TES3::Moon::fadeStartAngle;
		usertypeDefinition["index"] = sol::readonly_property(&TES3::Moon::index);
		usertypeDefinition["isRed"] = &TES3::Moon::moonIsRed;
		usertypeDefinition["phase"] = &TES3::Moon::phase;
		usertypeDefinition["sceneMoonFace"] = sol::readonly_property(&TES3::Moon::sgMoonNode);
		usertypeDefinition["sceneMoonRoot"] = sol::readonly_property(&TES3::Moon::sgNode);
		usertypeDefinition["sceneMoonShadow"] = sol::readonly_property(&TES3::Moon::sgShadowNode);
		usertypeDefinition["shadowEarlyFadeAngle"] = &TES3::Moon::shadowEarlyFadeAngle;
		usertypeDefinition["speed"] = &TES3::Moon::speed;
		usertypeDefinition["weatherController"] = sol::readonly_property(&TES3::Moon::weatherController);

		// Functions exposed as properties.
		usertypeDefinition["texture"] = sol::property(&TES3::Moon::getTexturePath, &TES3::Moon::setTexturePath);
	}
}
