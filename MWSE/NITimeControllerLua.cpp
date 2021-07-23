#include "NITimeControllerLua.h"

#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIRTTI.h"
#include "NIObjectNET.h"
#include "NITimeController.h"

namespace mwse {
	namespace lua {
		void bindNITimeController() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<NI::TimeController>("niTimeController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
			setUserdataForNIObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["frequency"] = &NI::TimeController::frequency;
			usertypeDefinition["phase"] = &NI::TimeController::phase;
			usertypeDefinition["lowKeyFrame"] = &NI::TimeController::lowKeyFrame;
			usertypeDefinition["highKeyFrame"] = &NI::TimeController::highKeyFrame;
			usertypeDefinition["startTime"] = &NI::TimeController::startTime;
			usertypeDefinition["lastTime"] = &NI::TimeController::lastTime;
			usertypeDefinition["target"] = sol::readonly_property(&NI::TimeController::target);
			usertypeDefinition["nextController"] = &NI::TimeController::nextController;

			// Basic function binding.
			usertypeDefinition["setTarget"] = &NI::TimeController::setTarget;
			usertypeDefinition["start"] = &NI::TimeController::start;
		}
	}
}
