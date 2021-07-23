#include "TES3FaderLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Fader.h"

namespace mwse {
	namespace lua {
		void bindTES3Fader() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Fader>("tes3fader");
			usertypeDefinition["new"] = sol::constructors<TES3::Fader(), TES3::Fader(float, bool)>();

			// Function bindings.
			usertypeDefinition["activate"] = &TES3::Fader::activate;
			usertypeDefinition["deactivate"] = &TES3::Fader::deactivate;
			usertypeDefinition["update"] = &TES3::Fader::updateForFrame;
			usertypeDefinition["setTexture"] = &TES3::Fader::setTexture;
			usertypeDefinition["updateMaterialProperty"] = &TES3::Fader::updateMaterialProperty;

			// Expose writing active state as mapping for activate/deactivate.
			usertypeDefinition["active"] = sol::property(&TES3::Fader::isActive, &TES3::Fader::setActive);

			// Expose fade as a fancier functions.
			usertypeDefinition["fadeIn"] = &TES3::Fader::fadeIn_lua;
			usertypeDefinition["fadeOut"] = &TES3::Fader::fadeOut_lua;
			usertypeDefinition["fadeTo"] = &TES3::Fader::fadeTo_lua;

			// Expose setColor fancier.
			usertypeDefinition["setColor"] = &TES3::Fader::setColor_lua;

			// Deprecated bindings. Kept for backwards compatibility.
			usertypeDefinition["removeMaterialProperty"] = &TES3::Fader::updateMaterialProperty;
		}
	}
}
