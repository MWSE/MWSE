#include "NISwitchNode.h"

#include "NINodeLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIDefines.h"
#include "NINode.h"
#include "NIRTTI.h"

namespace mwse::lua {
	void bindNISwitchNode() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::SwitchNode>("niSwitchNode");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<NI::Node, NI::AVObject, NI::ObjectNET, NI::Object>();
		setUserdataForNINode(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["switchIndex"] = sol::property(&NI::SwitchNode::getSwitchIndex, &NI::SwitchNode::setSwitchIndex);

		// Basic function binding.
		usertypeDefinition["getActiveChild"] = &NI::SwitchNode::getActiveChild;
		usertypeDefinition["getChildIndexByName"] = &NI::SwitchNode::getChildIndexByName;
	}
}
