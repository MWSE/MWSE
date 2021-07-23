#include "TES3MoonLua.h"

#include "LuaManager.h"

#include "TES3Moon.h"
#include "TES3WeatherController.h"

namespace mwse {
	namespace lua {
		void bindTES3Moon() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
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
			usertypeDefinition["shadowEarlyFadeAngle"] = &TES3::Moon::shadowEarlyFadeAngle;
			usertypeDefinition["speed"] = &TES3::Moon::speed;
			usertypeDefinition["weatherController"] = sol::readonly_property(&TES3::Moon::weatherController);

			// Functions exposed as properties.
			usertypeDefinition["texture"] = sol::property(&TES3::Moon::getTexturePath, &TES3::Moon::setTexturePath);
		}
	}
}
