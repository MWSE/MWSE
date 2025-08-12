//#include "TES3CollisionLua.h"

#include "LuaManager.h"

#include "TES3MobileObject.h"
#include "TES3Reference.h"

namespace mwse::lua {
	void bindTES3Collision() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::MobileObject::Collision>("tes3mobileObjectCollision");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		using Collision = TES3::MobileObject::Collision;
		usertypeDefinition["colliderRef"] = sol::readonly_property(&Collision::colliderRef);
		usertypeDefinition["colliderRoot"] = sol::readonly_property(&Collision::colliderRoot);
		usertypeDefinition["collisionType"] = sol::readonly_property(&Collision::collisionType);
		usertypeDefinition["objectPosAtCollision"] = sol::readonly_property(&Collision::objectPosAtCollision);
		usertypeDefinition["point"] = sol::readonly_property(&Collision::point);
		usertypeDefinition["time"] = sol::readonly_property(&Collision::time);
		usertypeDefinition["valid"] = sol::readonly_property(&Collision::valid);
		usertypeDefinition["velocity"] = sol::readonly_property(&Collision::velocity);

		// Access to other objects that need to be packaged.
		usertypeDefinition["normal"] = sol::readonly_property(&Collision::getNormal);
	}
}
