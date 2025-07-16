#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIDefines.h"
#include "NIAVObject.h"
#include "NICollisionGroup.h"

namespace mwse::lua {
	void bindNICollisionGroup() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::CollisionGroup>("niCollisionGroup");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["collidees"] = sol::readonly_property(&NI::CollisionGroup::collidees);
			usertypeDefinition["colliders"] = sol::readonly_property(&NI::CollisionGroup::colliders);
		}
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::CollisionGroup::Record>("niCollisionGroupRecord");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["collisionRoot"] = sol::readonly_property(&NI::CollisionGroup::Record::sgCollisionRoot);
			usertypeDefinition["object"] = sol::readonly_property(&NI::CollisionGroup::Record::sgObject);
		}
	}
}
