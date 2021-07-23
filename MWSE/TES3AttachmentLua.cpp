#include "TES3ActorLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIPointLight.h"

#include "TES3Attachment.h"
#include "TES3Cell.h"
#include "TES3Misc.h"
#include "TES3Spell.h"

namespace mwse {
	namespace lua {
		void bindTES3Attachment() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Bind TES3::LightAttachmentNode
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::LightAttachmentNode>("tes3lightNode");
				usertypeDefinition["new"] = sol::no_constructor;

				// Access to node properties.
				usertypeDefinition["light"] = &TES3::LightAttachmentNode::light;
				usertypeDefinition["value"] = &TES3::LightAttachmentNode::flickerPhase;
			}

			// Bind TES3::LockAttachmentNode
			{
				// Start our usertype. We must finish this with state.set_usertype.
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
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::TravelDestination>("tes3travelDestinationNode");
				usertypeDefinition["new"] = sol::no_constructor;

				// Access to other objects that need to be packaged.
				usertypeDefinition["cell"] = &TES3::TravelDestination::cell;
				usertypeDefinition["marker"] = &TES3::TravelDestination::destination;
			}
		}
	}
}
