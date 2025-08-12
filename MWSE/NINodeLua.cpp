#include "NINodeLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIBillboardNode.h"
#include "NINode.h"
#include "NISortAdjustNode.h"

namespace mwse::lua {
	void bindNINode() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::Node.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Node>("niNode");
			usertypeDefinition["new"] = &NI::Node::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNINode(usertypeDefinition);
		}

		// Binding for NI::BillboardNode.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::BillboardNode>("niBillboardNode");
			usertypeDefinition["new"] = &NI::BillboardNode::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Node, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNINode(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["mode"] = sol::property(&NI::BillboardNode::getMode, &NI::BillboardNode::setMode);

			// Basic function binding.
			usertypeDefinition["rotateToCamera"] = &NI::BillboardNode::rotateToCamera;
		}

		// Binding for NI::SortAdjustNode.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::SortAdjustNode>("niSortAdjustNode");
			usertypeDefinition["new"] = &NI::SortAdjustNode::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Node, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNINode(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["accumulator"] = &NI::SortAdjustNode::accumulator;
			usertypeDefinition["sortingMode"] = &NI::SortAdjustNode::sortingMode;
		}
	}
}
