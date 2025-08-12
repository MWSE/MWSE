#include "TES3MobileProjectileLua.h"

#include "LuaManager.h"
#include "TES3MobileObjectLua.h"

#include "TES3MobileProjectile.h"
#include "TES3MobileSpellProjectile.h"
#include "TES3MobileActor.h"
#include "TES3Weapon.h"

namespace mwse::lua {
	void bindTES3MobileProjectile() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Define base mobile projectiles.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::MobileProjectile>("tes3mobileProjectile");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::MobileObject>();
			setUserdataForTES3MobileProjectile(usertypeDefinition);
		}

		// Define spell projectiles.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::MobileSpellProjectile>("tes3mobileSpellProjectile");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::MobileProjectile, TES3::MobileObject>();
			setUserdataForTES3MobileProjectile(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["rotationSpeed"] = &TES3::MobileSpellProjectile::rotationSpeed;
			usertypeDefinition["spellInstanceSerial"] = sol::readonly_property(&TES3::MobileSpellProjectile::spellInstanceSerial);

			// Functions exposed as properties.
			usertypeDefinition["spellInstance"] = sol::readonly_property(&TES3::MobileSpellProjectile::getInstance);

			// Basic function bindings.
			usertypeDefinition["explode"] = &TES3::MobileSpellProjectile::explodeDeferred;
		}
	}
}
