#include "TES3ActorLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIPointLight.h"

#include "TES3Attachment.h"
#include "TES3Cell.h"
#include "TES3Misc.h"
#include "TES3Spell.h"

namespace mwse::lua {
	void bindTES3Attachment() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Bind TES3::LightAttachmentNode
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::LightAttachmentNode>("tes3lightNode");
			usertypeDefinition["new"] = sol::no_constructor;

			// Access to node properties.
			usertypeDefinition["light"] = sol::property(&TES3::LightAttachmentNode::getLight, &TES3::LightAttachmentNode::setLight);
			usertypeDefinition["value"] = &TES3::LightAttachmentNode::flickerPhase;
		}

		// Bind TES3::LockAttachmentNode
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::LockAttachmentNode>("tes3lockNode");
			usertypeDefinition["new"] = sol::no_constructor;

			// Access to node properties.
			usertypeDefinition["level"] = &TES3::LockAttachmentNode::lockLevel;
			usertypeDefinition["locked"] = &TES3::LockAttachmentNode::locked;

			// Access to other objects that need to be packaged.
			usertypeDefinition["key"] = sol::property(&TES3::LockAttachmentNode::getKey, &TES3::LockAttachmentNode::setKey);
			usertypeDefinition["trap"] = &TES3::LockAttachmentNode::trap;
		}

		// Bind TES3::TravelDestination
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::TravelDestination>("tes3travelDestinationNode");
			usertypeDefinition["new"] = sol::no_constructor;

			// Access to other objects that need to be packaged.
			usertypeDefinition["cell"] = &TES3::TravelDestination::cell;
			usertypeDefinition["marker"] = &TES3::TravelDestination::destination;
		}
	}
}
