#include "TES3MagicEffectInstanceLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Armor.h"
#include "TES3Inventory.h"
#include "TES3Light.h"
#include "TES3MagicEffectInstance.h"
#include "TES3Reference.h"
#include "TES3Weapon.h"

namespace mwse {
	namespace lua {
		void bindTES3MagicEffectInstance() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::MagicEffectInstance>("tes3magicEffectInstance");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["createdData"] = &TES3::MagicEffectInstance::createdData;
			usertypeDefinition["cumulativeMagnitude"] = &TES3::MagicEffectInstance::cumulativeMagnitude;
			usertypeDefinition["lastUsedArmor"] = &TES3::MagicEffectInstance::lastUsedArmor;
			usertypeDefinition["lastUsedEnchItem"] = &TES3::MagicEffectInstance::lastUsedEnchItem;
			usertypeDefinition["lastUsedLight"] = &TES3::MagicEffectInstance::lastUsedLight;
			usertypeDefinition["lastUsedShield"] = &TES3::MagicEffectInstance::lastUsedShield;
			usertypeDefinition["lastUsedWeapon"] = &TES3::MagicEffectInstance::lastUsedWeapon;
			usertypeDefinition["magnitude"] = &TES3::MagicEffectInstance::magnitude;
			usertypeDefinition["resistedPercent"] = &TES3::MagicEffectInstance::resistedPercent;
			usertypeDefinition["state"] = &TES3::MagicEffectInstance::state;
			usertypeDefinition["target"] = &TES3::MagicEffectInstance::target;
			usertypeDefinition["timeActive"] = &TES3::MagicEffectInstance::timeActive;
			usertypeDefinition["visual"] = &TES3::MagicEffectInstance::visual;
		}
	}
}
