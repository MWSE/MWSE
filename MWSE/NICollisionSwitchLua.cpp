#include "NISwitchNodeLua.h"

#include "NINodeLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIDefines.h"
#include "NICollisionSwitch.h"
#include "NIRTTI.h"

namespace mwse::lua {
	void bindNICollisionSwitch() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::CollisionSwitch>("niCollisionSwitch");
		usertypeDefinition["new"] = &NI::CollisionSwitch::create;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<NI::Node, NI::AVObject, NI::ObjectNET, NI::Object>();
		setUserdataForNINode(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["collisionActive"] = sol::property(&NI::CollisionSwitch::getCollisionActive, &NI::CollisionSwitch::setCollisionActive);
	}
}
