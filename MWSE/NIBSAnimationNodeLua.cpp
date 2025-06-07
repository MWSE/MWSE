#include "NIBSAnimationNodeLua.h"
#include "NINodeLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIBSAnimationNode.h"

namespace mwse::lua {
	void bindNIBSAnimationNode() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Binding for NI::BSAnimationNode.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::BSAnimationNode>("niBSAnimationNode");
			usertypeDefinition["new"] = &NI::BSAnimationNode::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Node, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNINode(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["animationFlags"] = &NI::BSAnimationNode::animationFlags;
		}

		// Binding for NI::BSParticleNode.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::BSParticleNode>("niBSParticleNode");
			usertypeDefinition["new"] = &NI::BSParticleNode::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::BSAnimationNode, NI::Node, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNINode(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["animationFlags"] = &NI::BSParticleNode::animationFlags;
			usertypeDefinition["followMode"] = sol::property(&NI::BSParticleNode::getFollowMode, &NI::BSParticleNode::setFollowMode);
		}
	}
}
