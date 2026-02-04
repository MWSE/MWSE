#include "NIBoundingVolumeLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIBound.h"

namespace mwse::lua {
	void bindNIBoundingVolume() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::SphereBound>("niBound"); // Legacy naming defaults to spheres.
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["center"] = &NI::SphereBound::center;
		usertypeDefinition["radius"] = &NI::SphereBound::radius;
	}
}
