#include "TES3PlayerAnimationControllerLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3PlayerAnimationController.h"

namespace mwse {
	namespace lua {
		void bindTES3PlayerAnimationController() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::PlayerAnimationController>("tes3playerAnimationController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::ActorAnimationController>();
		}
	}
}
