#include "TES3VFXManagerLua.h"

#include "TES3Reference.h"
#include "TES3MagicSourceInstance.h"
#include "TES3VFXManager.h"

#include "NINode.h"

#include "LuaManager.h"

namespace mwse::lua {
	void bindTES3VFX() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Bindings for TES3::VFXManager
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::VFXManager>("tes3vfxManager");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property bindings.
			usertypeDefinition["data"] = sol::readonly_property(&TES3::VFXManager::vfxNodes);
			usertypeDefinition["worldVFXRoot"] = sol::readonly_property(&TES3::VFXManager::worldVFXRoot);

			// Basic function bindings.
			usertypeDefinition["reset"] = &TES3::VFXManager::reset;
		}

		// Bindings for TES3::VFX
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::VFX>("tes3vfx");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property bindings.
			usertypeDefinition["age"] = &TES3::VFX::age;
			usertypeDefinition["attachNode"] = &TES3::VFX::createdFromNode;
			usertypeDefinition["beginKeyTime"] = &TES3::VFX::beginKeyTime;
			usertypeDefinition["effectNode"] = &TES3::VFX::clonedEffectNode;
			usertypeDefinition["effectObject"] = &TES3::VFX::effectObject;
			usertypeDefinition["endKeyTime"] = &TES3::VFX::endKeyTime;
			usertypeDefinition["expired"] = &TES3::VFX::expired;
			usertypeDefinition["id"] = sol::readonly_property(&TES3::VFX::vfxId);
			usertypeDefinition["keyTime"] = &TES3::VFX::keyTime;
			usertypeDefinition["maxAge"] = &TES3::VFX::maxAge;
			usertypeDefinition["position"] = &TES3::VFX::position;
			usertypeDefinition["sourceInstance"] = sol::readonly_property(&TES3::VFX::getSourceInstance);
			usertypeDefinition["sourceInstanceSerial"] = sol::readonly_property(&TES3::VFX::sourceInstanceSerial);
			usertypeDefinition["target"] = &TES3::VFX::target;
			usertypeDefinition["verticalOffset"] = &TES3::VFX::verticalOffset;
		}
	}
}
