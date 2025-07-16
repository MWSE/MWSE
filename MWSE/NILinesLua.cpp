#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIDefines.h"
#include "NIRTTI.h"
#include "NILines.h"
#include "NILinesData.h"

namespace mwse::lua {
	void bindNILines() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::Lines.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Lines>("niLines");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Geometry, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNIAVObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["data"] = sol::property(&NI::Lines::getModelData, &NI::Lines::setModelData);
		}
	}
}
