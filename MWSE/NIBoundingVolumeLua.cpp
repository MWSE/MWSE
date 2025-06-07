#include "NIBoundingVolumeLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIBound.h"

namespace mwse::lua {
	void bindNIBoundingVolume() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::Bound>("niBound");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["center"] = &NI::Bound::center;
		usertypeDefinition["radius"] = &NI::Bound::radius;
	}
}
